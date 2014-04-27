#!/usr/bin/perl
use warnings;
use strict;
use open qw{:std :utf8};

@ARGV >= 3 or die "Usage: $0 UniLib_version Unicode_version UnicodeData.xz\n";
my $UniLibVersion = shift @ARGV;
my $UnicodeVersion = shift @ARGV;
my $UnicodeData = shift @ARGV;

my $N = 0x110000;

# Load UnicodeData into cat and othercase.
my %cat = (name=>'CATEGORY', data=>[('_Cn') x $N]);
my %othercase = (name=>'OTHERCASE', data=>[(0) x $N]);
my %ccc = (name=>'CCC', data=>[(0) x $N]);
my %decomposition = (name=>'DECOMPOSITION', data=>[(0) x $N], decomposition=>[], rawdata=>[0]);
my @data = (\%cat, \%othercase, \%ccc, \%decomposition);

open (my $f, "-|", "xzcat $UnicodeData") or die "Cannot open 'xzcat $UnicodeData': $!";
while (<$f>) {
  chomp;
  my @parts = split /;/, $_, -1;
  die "Bad line $_ in UnicodeData.txt" unless @parts == 15;

  my ($code, $name, $cat, $ccc, $decomposition, $upper, $lower, $title) = @parts[0, 1, 2, 3, 5, 12, 13, 14];
  my $othercase = $cat =~ /L[ut]/ ? $lower : $cat =~ /Ll/ ? $upper : "";

  $code = hex($code);
  $othercase = length $othercase ? hex($othercase) - $code : 0;

  my $last_code = $code;
  if ($name =~ /^<(.*), First>$/) {
    my $range_name = $1;
    chomp (my $next_line = <$f>);
    my @next_parts = split /;/, $next_line, -1;
    my ($next_code, $next_name) = @next_parts[0, 1];
    $next_name =~ /^<$range_name, Last>$/ or die "Unrecognized end '$next_name' of range $range_name";
    $othercase == 0 or die "First element of range $range_name has othercase";
    $ccc == 0 or die "First element of range $range_name has ccc";
    length($decomposition) == 0 or die "First element of range $range_name has decomposition";
    $last_code = hex($next_code);
  }

  for (; $code <= $last_code; $code++) {
    $cat{data}->[$code] = "_$cat";
    $othercase{data}->[$code] = $othercase;
    $ccc{data}->[$code] = $ccc;
    $decomposition{decomposition}->[$code] = [$decomposition =~ s/^<[^>]*>\s*// ? 1 : 0] if length($decomposition);
    push @{$decomposition{decomposition}->[$code]}, map(hex, split /\s+/, $decomposition)  if length($decomposition);
  }
}
close $f;

# Method for splitting long lines
sub split_long {
  my ($line) = @_;
  my @lines = split /\n/, $line;
  foreach my $line (@lines) {
    my $result = '';
    while (length $line >= 2000) {
      my $comma = rindex($line, ',', 1999) + 1;
      $result .= substr($line, 0, $comma) . "\n    ";
      $line = substr($line, $comma);
    }
    $line = $result . $line;
  }
  return join("\n", @lines);
}

# Fill decomposition data
sub skip_first {
  shift @_;
  return @_;
}

sub decompose {
  my ($code) = @_;
  return $code unless $decomposition{decomposition}->[$code];
  return map {decompose($_)} skip_first(@{$decomposition{decomposition}->[$code]});
}

for (my $code = 0; $code < $N; $code++) {
  next unless $decomposition{decomposition}->[$code];
  my @decomposition = map {decompose($_)} skip_first(@{$decomposition{decomposition}->[$code]});
  $decomposition{data}->[$code] = scalar(@{$decomposition{rawdata}});
  push @{$decomposition{rawdata}}, scalar(@decomposition) * 2 + $decomposition{decomposition}->[$code]->[0];
  foreach my $chr (@decomposition) {
    push @{$decomposition{rawdata}}, $chr & 0xFF;
    push @{$decomposition{rawdata}}, ($chr >> 8) & 0xFF;
    push @{$decomposition{rawdata}}, ($chr >> 16) & 0xFF;
  }
}
$decomposition{rawdata} = split_long("{\n  " . join(",", @{$decomposition{rawdata}}) . "\n}");

# Generate blocks of length 256 for cat and othercase.
foreach my $data_ref (@data) {
  my (@blocks, %blocks, @indices);
  for (my $b = 0; $b < $N; $b += 256) {
    my $block = "{" . join(",", @{$data_ref->{data}}[$b..$b+255]) . "}";
    if (not exists $blocks{$block}) {
      $blocks{$block} = @blocks;
      push @blocks, $block;
    }
    push @indices, $blocks{$block};
  }
  $data_ref->{indices} = split_long("{\n  " . join(",", @indices) . "\n}");
  $data_ref->{blocks} = split_long("{\n  " . join(",\n  ", @blocks) . "\n}");
}

# Replace templates in given file.
while (<>) {
  s/\$UNILIB_VERSION/$UniLibVersion/g;
  s/\$UNICODE_VERSION/$UnicodeVersion/g;
  foreach my $data_ref (@data) {
    s/\$$data_ref->{name}_INDICES/$data_ref->{indices}/eg;
    s/\$$data_ref->{name}_BLOCKS/$data_ref->{blocks}/eg;
  }
  foreach my $data_ref (\%decomposition) {
    s/\$$data_ref->{name}_DATA/$data_ref->{rawdata}/eg;
  }
  print;
}
