#!/usr/bin/perl
use warnings;
use strict;
use open qw{:std :utf8};

@ARGV >= 4 or die "Usage: $0 UniLib_version Unicode_version UnicodeData.xz CompositionExclusions.xz\n";
my $UniLibVersion = shift @ARGV;
my $UnicodeVersion = shift @ARGV;
my $UnicodeData = shift @ARGV;
my $CompositionExclusion = shift @ARGV;

my $N = 0x110000;

# Load UnicodeData
my %cat = (name=>'CATEGORY', data=>[('_Cn') x $N]);
my %othercase = (name=>'OTHERCASE', data=>[(0) x $N]);
my %ccc = (name=>'CCC', data=>[(0) x $N]);
my %composition = (name=>'COMPOSITION', data=>[], composition=>[], rawdata=>[0]);
my %decomposition = (name=>'DECOMPOSITION', data=>[], decomposition=>[], rawdata=>[0]);
my @data = (\%cat, \%othercase, \%ccc, \%composition, \%decomposition);

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
    next unless length($decomposition);
    my $kanonical = $decomposition =~ s/^<[^>]*>\s*// ? 1 : 0;
    $decomposition{decomposition}->[$code] = [$kanonical, map(hex, split /\s+/, $decomposition)];
  }
}
close $f;

# Load CompositionExclusions and fill composition data
my %excluded = ();
open ($f, "-|", "xzcat $CompositionExclusion") or die "Cannot open 'xzcat $CompositionExclusion': $!";
while (<$f>) {
  chomp;
  s/\s*(#.*)?$//;
  next unless length;
  /^([\da-fA-F]{4,5})$/ or die "Bad line $_ in CompositionExclusions";
  $excluded{hex($1)} = 1;
}
close $f;

for (my $code = 0; $code < $N; $code++) {
  next unless $decomposition{decomposition}->[$code];     # skip no decomposition
  next if $decomposition{decomposition}->[$code]->[0];    # skip kanonical decomposition
  next if @{$decomposition{decomposition}->[$code]} == 2; # skip singleton decomposition
  die "Unexpected non-kanonical decomposition for $code" if @{$decomposition{decomposition}->[$code]} != 3;
  next if $ccc{data}->[$code] != 0;                       # skip non-starter characters
  next if $ccc{data}->[$decomposition{decomposition}->[$code]->[1]] != 0; # skip non-starter decomposition
  next if $excluded{$code};                               # skip composition exclusion
  push @{$composition{composition}->[$decomposition{decomposition}->[$code]->[1]]}, [$decomposition{decomposition}->[$code]->[2], $code];
}

for (my $code = 0; $code <= $N; $code++) {
  my @composition;
  @composition = sort {$a->[0] <=> $b->[0]} @{$composition{composition}->[$code]} if $composition{composition}->[$code];
  $composition{data}->[$code] = scalar(@{$composition{rawdata}});
  push @{$composition{rawdata}}, map {@{$_}} @composition;
}
$composition{rawdata} = "{\n  " . join(",", @{$composition{rawdata}}) . "\n}";

# Fill decomposition data
sub skip_first {
  shift @_;
  return @_;
}

sub decompose {
  my ($code, $kanonical) = @_;
  return $code unless $decomposition{decomposition}->[$code] && ($kanonical || !$decomposition{decomposition}->[$code]->[0]);
  return map {decompose($_, $kanonical)} skip_first(@{$decomposition{decomposition}->[$code]});
}

for (my $code = 0; $code <= $N; $code++) {
  my ($kanonical, $further_kanonical, @decomposition) = (0, 0);
  if ($decomposition{decomposition}->[$code]) {
    $kanonical = $decomposition{decomposition}->[$code]->[0];
    @decomposition = map {decompose($_, $kanonical)} skip_first(@{$decomposition{decomposition}->[$code]});
    $further_kanonical = !$kanonical && @decomposition != map {decompose($_, 1)} skip_first(@{$decomposition{decomposition}->[$code]});
    die "No decomposition" if not @decomposition;
    die "Identity decomposition" if @decomposition == 1 && $decomposition[0] eq $code;
  }
  $decomposition{data}->[$code] = 4 * scalar(@{$decomposition{rawdata}}) + 2 * $further_kanonical + $kanonical;
  push @{$decomposition{rawdata}}, @decomposition;
}
$decomposition{rawdata} = "{\n  " . join(",", @{$decomposition{rawdata}}) . "\n}";

# Generate blocks of length 256 (or 257 for composition and decomposition).
foreach my $data_ref (@data) {
  my (@blocks, %blocks, @indices);
  my $bsize = $data_ref->{name} =~ /(DE)?COMPOSITION/ ? 257 : 256;
  for (my $b = 0; $b < $N; $b += 256) {
    my $block = "{" . join(",", $bsize==257 && $data_ref->{data}->[$b]==$data_ref->{data}->[$b+$bsize-1] ? (0)x257 : @{$data_ref->{data}}[$b..$b+$bsize-1]) . "}";
    if (not exists $blocks{$block}) {
      $blocks{$block} = @blocks;
      push @blocks, $block;
    }
    push @indices, $blocks{$block};
  }
  $data_ref->{indices} = "{\n  " . join(",", @indices) . "\n}";
  $data_ref->{blocks} = "{\n  " . join(",\n  ", @blocks) . "\n}";
}

# Split long lines in generated tables.
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

foreach my $data_ref (@data) {
  $data_ref->{indices} = split_long($data_ref->{indices});
  $data_ref->{blocks} = split_long($data_ref->{blocks});
}
$composition{rawdata} = split_long($composition{rawdata});
$decomposition{rawdata} = split_long($decomposition{rawdata});

# Replace templates in given file.
while (<>) {
  s/\$UNILIB_VERSION/$UniLibVersion/g;
  s/\$UNICODE_VERSION/$UnicodeVersion/g;
  foreach my $data_ref (@data) {
    s/\$$data_ref->{name}_INDICES/$data_ref->{indices}/eg;
    s/\$$data_ref->{name}_BLOCKS/$data_ref->{blocks}/eg;
  }
  foreach my $data_ref (\%composition, \%decomposition) {
    s/\$$data_ref->{name}_DATA/$data_ref->{rawdata}/eg;
  }
  print;
}
