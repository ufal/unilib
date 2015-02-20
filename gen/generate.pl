#!/usr/bin/perl

# This file is part of UniLib <http://github.com/ufal/unilib/>.
#
# Copyright 2014 Institute of Formal and Applied Linguistics, Faculty of
# Mathematics and Physics, Charles University in Prague, Czech Republic.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

use warnings;
use strict;
use open qw{:std :utf8};

@ARGV >= 4 && !(@ARGV % 2) or die "Usage: $0 UniLib_version Unicode_version UnicodeData.xz CompositionExclusions.xz [input_file output_file]*\n";
my $UniLibVersion = shift @ARGV;
my $UnicodeVersion = shift @ARGV;
my $UnicodeData = shift @ARGV;
my $CompositionExclusion = shift @ARGV;

$UniLibVersion =~ /^(\d+)\.(\d+)\.(\d+)(?:-(.+))?$/ or die "Cannot parse UNILIB version $UniLibVersion";
my %versions = (
  UNILIB_VERSION => $UniLibVersion,
  UNILIB_MAJOR_VERSION => $1,
  UNILIB_MINOR_VERSION => $2,
  UNILIB_PATCH_VERSION => $3,
  UNILIB_PRERELEASE_VERSION => '"'.($4 // "").'"',
  UNICODE_VERSION => $UnicodeVersion
);

my $N = 0x110000;

# Load UnicodeData
my %cat = (name=>'CATEGORY', data=>[('_Cn') x $N]);
my %othercase = (name=>'OTHERCASE', data=>[(0) x $N]);
my %ccc = (name=>'CCC', data=>[(0) x $N]);
my %composition = (name=>'COMPOSITION', data=>[], composition=>[], rawdata=>[0]);
my %decomposition = (name=>'DECOMPOSITION', data=>[], decomposition=>[], rawdata=>[0]);
my %combining_mark = (name=>'COMBINING_MARK', data=>[(0) x $N]);
my %stripped = (name=>'STRIPPED', data=>[(0) x $N], rawdata=>[0], rawmap=>{});
my @data = (\%cat, \%othercase, \%ccc, \%composition, \%decomposition, \%combining_mark, \%stripped);

open (my $f, "-|", "xzcat $UnicodeData") or die "Cannot open 'xzcat $UnicodeData': $!";
while (<$f>) {
  chomp;
  my @parts = split /;/, $_, -1;
  die "Bad line $_ in UnicodeData.txt" unless @parts == 15;

  my ($code, $name, $cat, $ccc, $decomposition, $upper, $lower, $title) = @parts[0, 1, 2, 3, 5, 12, 13, 14];
  $code = hex($code);

  my $othercase;
  $title = "" if hex($title) == $code;
  if (length($lower) && !length($upper) && !length($title)) { $othercase = hex($lower) * 256 + 1; }
  elsif (!length($lower) && length($upper) && $upper eq $title) { $othercase = hex($upper) * 256 + 2; }
  elsif (!length($lower) && !length($upper) && !length($title)) { $othercase = 0; }
  elsif (length($lower) && length($upper) && !length($title)) { $othercase = hex($lower) * 256 + 3; }
  elsif (!length($lower) && length($upper) && length($title)) { $othercase = hex($upper) * 256 + 4; }
  elsif (length($lower) && !length($upper) && length($title)) { $othercase = hex($title) * 256 + 5; }
  else { die "Cannot parse lower/upper/title in UCD line '$_'"; }

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
    my $kompatibility = $decomposition =~ s/^<[^>]*>\s*// ? 1 : 0;
    $decomposition{decomposition}->[$code] = [$kompatibility, map(hex, split /\s+/, $decomposition)];
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
  next if $decomposition{decomposition}->[$code]->[0];    # skip kompatibility decomposition
  next if @{$decomposition{decomposition}->[$code]} == 2; # skip singleton decomposition
  die "Unexpected non-kompatibility decomposition for $code" if @{$decomposition{decomposition}->[$code]} != 3;
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
  my ($code, $kompatibility) = @_;
  return $code unless $decomposition{decomposition}->[$code] && ($kompatibility || !$decomposition{decomposition}->[$code]->[0]);
  return map {decompose($_, $kompatibility)} skip_first(@{$decomposition{decomposition}->[$code]});
}

for (my $code = 0; $code <= $N; $code++) {
  my ($kompatibility, $further_kompatibility, @decomposition) = (0, 0);
  if ($decomposition{decomposition}->[$code]) {
    $kompatibility = $decomposition{decomposition}->[$code]->[0];
    @decomposition = map {decompose($_, $kompatibility)} skip_first(@{$decomposition{decomposition}->[$code]});
    $further_kompatibility = join(" ",@decomposition) ne join(" ",map {decompose($_, 1)} skip_first(@{$decomposition{decomposition}->[$code]})) ? 1 : 0;
    die "No decomposition" if not @decomposition;
    die "Identity decomposition" if @decomposition == 1 && $decomposition[0] eq $code;
  }
  $decomposition{data}->[$code] = 4 * scalar(@{$decomposition{rawdata}}) + 2 * $further_kompatibility + $kompatibility;
  push @{$decomposition{rawdata}}, @decomposition;
}
$decomposition{rawdata} = "{\n  " . join(",", @{$decomposition{rawdata}}) . "\n}";

# Fill combining mark data
for (my $code = 0; $code < $N; $code++) {
  $combining_mark{data}->[$code] = $cat{data}->[$code] =~ /^_M/ ? 1 : 0;
}

# Fill stripped combining marks data
for (my $code = 0; $code < $N; $code++) {
  # Is the character not a combining mark and has nonkompatibility decomposition?
  if ($cat{data}->[$code] !~ /^_M/ && $decomposition{decomposition}->[$code] && !$decomposition{decomposition}->[$code]->[0]) {
    my @decomposition = map {decompose($_, 0)} skip_first(@{$decomposition{decomposition}->[$code]});
    my @stripped = grep {$cat{data}->[$_] !~ /^_M/} @decomposition;
    if (scalar(@stripped) < scalar(@decomposition)) {
      die "Bad stripped combining marks decomposition of $code: " . join(" ", @stripped) if @stripped != 1 || $stripped[0] == $code;
      if (not exists $stripped{rawmap}->{$stripped[0]}) {
        $stripped{rawmap}->{$stripped[0]} = @{$stripped{rawdata}};
        push @{$stripped{rawdata}}, $stripped[0];
      }
      $stripped{data}->[$code] = $stripped{rawmap}->{$stripped[0]};
    }
  }
}
$stripped{rawdata} = "{\n  " . join(",", @{$stripped{rawdata}}) . "\n}";

# Generate blocks of suitable length (32, 256 or 257).
foreach my $data_ref (@data) {
  my (@blocks, %blocks, @indices);
  my $bsize = $data_ref->{name} =~ /(DE)?COMPOSITION/ ? 257 : 256;
  for (my $b = 0; $b < $N; $b += 256) {
    my $block = "{" . join(",",
      $data_ref->{name} =~ /(DE)?COMPOSITION/ ? $data_ref->{data}->[$b]==$data_ref->{data}->[$b+256] ? (0)x257 : @{$data_ref->{data}}[$b..$b+256] :
      $data_ref->{name} =~ /COMBINING_MARK/ ? map(ord, split(//, pack("b*", join("", @{$data_ref->{data}}[$b..$b+255])))) :
      @{$data_ref->{data}}[$b..$b+255]
    ) . "}";
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
foreach my $data_ref (\%composition, \%decomposition, \%stripped) {
  $data_ref->{rawdata} = split_long($data_ref->{rawdata});
}

# Replace templates in given files.
while (@ARGV) {
  my $input_file = shift @ARGV;
  my $output_file = shift @ARGV;

  open (my $input, "<", $input_file) or die "Cannot open file $input_file: $!";
  open (my $output, ">", $output_file) or die "Cannot open file $output_file: $!";
  while (<$input>) {
    foreach my $v (keys %versions) {
      s/\$$v/$versions{$v}/g;
    }
    foreach my $data_ref (@data) {
      s/\$$data_ref->{name}_INDICES/$data_ref->{indices}/eg;
      s/\$$data_ref->{name}_BLOCKS/$data_ref->{blocks}/eg;
    }
    foreach my $data_ref (\%composition, \%decomposition, \%stripped) {
      s/\$$data_ref->{name}_DATA/$data_ref->{rawdata}/eg;
    }
    print $output $_;
  }
  close $input;
  close $output;
}
