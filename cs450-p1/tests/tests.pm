use strict;
use warnings;
use tests::Algorithm::Diff;
use File::Temp 'tempfile';
use Fcntl qw(SEEK_SET SEEK_CUR);
use Getopt::Std;

sub fail;
sub pass;

my $ignore_white_space = 0;
my $ignore_case = 0;
our ($opt_i, $opt_c);
getopts('ic');

if (defined ($opt_i)) {
    $ignore_white_space = 1;
}
if (defined ($opt_c)) {
    $ignore_case = 1;
}

die if @ARGV != 2;
our ($test, $src_dir) = @ARGV;

my ($msg_file) = tempfile ();
select ($msg_file);

# Generic testing.

sub check_expected {
    my ($expected) = pop @_;
    my (@output) = read_text_file ("$test.output");
    common_checks ("run", @output);
    compare_output ("run", \@output, $expected);
}

sub common_checks {
    my ($run, @output) = @_;
    fail "\u$run produced no output at all\n" if @output == 0;
    check_for_keyword ($run, "FAIL", @output);
}

sub check_for_keyword {
    my ($run, $keyword, @output) = @_;

    my ($kw_line) = grep (/$keyword/, @output);
    return unless defined $kw_line;

    # Most output lines are prefixed by (test-name).  Eliminate this
    # from our message for brevity.
    $kw_line =~ s/^\([^\)]+\)\s+//;
    print "$run: $kw_line\n";

    fail;
}

sub compare_output {
    my ($run) = shift @_;
    my ($expected) = pop @_;
    my ($output) = pop @_;

    my (@output) = @$output;
    fail "\u$run didn't produce any output" if !@output;

    my @testoutput = @output;

    if ($ignore_white_space) {
        @testoutput = map { local $_ = $_; s/\s+/ /g; $_ } @output;
        @testoutput = map { local $_ = $_; s/^\s+//g; $_ } @testoutput;
        @testoutput = map { local $_ = $_; s/\s+$//g; $_ } @testoutput;
    }
    if ($ignore_case) {
        @testoutput = map { local $_ = $_; $_ = lc $_; $_ } @testoutput;
    }

    my ($msg);

    # Compare actual output against each allowed output.
    if (ref ($expected) eq 'ARRAY') {
	    my ($i) = 0;
	    $expected = {map ((++$i => $_), @$expected)};
    }
    foreach my $key (keys %$expected) {
	    my (@expected) = split ("\n", $expected->{$key});

	    $msg .= "Acceptable output:\n";
	    $msg .= join ('', map ("  $_\n", @expected));

        my @testexpected = @expected;
        if ($ignore_white_space) {
            @testexpected = map { local $_ = $_; s/\s+/ /g; $_ } @expected;
            @testexpected = map { local $_ = $_; s/^\s+//g; $_ } @testexpected;
            @testexpected = map { local $_ = $_; s/\s+$//g; $_ } @testexpected;
        }
        if ($ignore_case) {
            @testexpected = map { local $_ = $_; $_ = lc $_; $_ } @testexpected;
        }

	    # Check whether actual and expected match.
	    # If it's a perfect match, we're done.
	    if ($#testoutput == $#testexpected) {
	        my ($eq) = 1;
	        for (my ($i) = 0; $i <= $#testexpected; $i++) {
		        $eq = 0 if $testoutput[$i] ne $testexpected[$i];
	        }
	        return $key if $eq;
	    }

	    # They differ.  Output a diff.
	    my (@diff) = "";
	    my ($d) = Algorithm::Diff->new (\@expected, \@output);
	    while ($d->Next ()) {
	        my ($ef, $el, $af, $al) = $d->Get (qw (min1 max1 min2 max2));
	        if ($d->Same ()) {
		        push (@diff, map ("  $_\n", $d->Items (1)));
	        } else {
		        push (@diff, map ("- $_\n", $d->Items (1))) if $d->Items (1);
		        push (@diff, map ("+ $_\n", $d->Items (2))) if $d->Items (2);
	        }
	    }

	    $msg .= "Differences in `diff -u' format:\n";
	    $msg .= join ('', @diff);
    }

    # Failed to match.  Report failure.
    fail "Test output failed to match any acceptable form.\n\n$msg";
}

# compare_files ($A, $A_SIZE, $B, $B_SIZE, $NAME, $VERBOSE)
#
# Compares $A_SIZE bytes in $A to $B_SIZE bytes in $B.
# ($A and $B are handles.)
# If their contents differ, prints a brief message describing
# the differences, using $NAME to identify the file.
# The message contains more detail if $VERBOSE is nonzero.
# Returns 1 if the contents are identical, 0 otherwise.
sub compare_files {
    my ($a, $a_size, $b, $b_size, $name, $verbose) = @_;
    my ($ofs) = 0;
    select(STDOUT);
    for (;;) {
	my ($a_amt) = $a_size >= 1024 ? 1024 : $a_size;
	my ($b_amt) = $b_size >= 1024 ? 1024 : $b_size;
	my ($a_data, $b_data);
	if (!defined (sysread ($a, $a_data, $a_amt))
	    || !defined (sysread ($b, $b_data, $b_amt))) {
	    die "reading $name: $!\n";
	}

	my ($a_len) = length $a_data;
	my ($b_len) = length $b_data;
	last if $a_len == 0 && $b_len == 0;

	if ($a_data ne $b_data) {
	    my ($min_len) = $a_len < $b_len ? $a_len : $b_len;
	    my ($diff_ofs);
	    for ($diff_ofs = 0; $diff_ofs < $min_len; $diff_ofs++) {
		last if (substr ($a_data, $diff_ofs, 1)
			 ne substr ($b_data, $diff_ofs, 1));
	    }

	    printf "\nFile $name differs from expected "
	      . "starting at offset 0x%x.\n", $ofs + $diff_ofs;
	    if ($verbose ) {
		print "Expected contents:\n";
		hex_dump (substr ($a_data, $diff_ofs, 64), $ofs + $diff_ofs);
		print "Actual contents:\n";
		hex_dump (substr ($b_data, $diff_ofs, 64), $ofs + $diff_ofs);
	    }
	    return 0;
	}

	$ofs += $a_len;
	$a_size -= $a_len;
	$b_size -= $b_len;
    }
    return 1;
}

# hex_dump ($DATA, $OFS)
#
# Prints $DATA in hex and text formats.
# The first byte of $DATA corresponds to logical offset $OFS
# in whatever file the data comes from.
sub hex_dump {
    my ($data, $ofs) = @_;

    if ($data eq '') {
	printf "  (File ends at offset %08x.)\n", $ofs;
	return;
    }

    my ($per_line) = 16;
    while ((my $size = length ($data)) > 0) {
	my ($start) = $ofs % $per_line;
	my ($end) = $per_line;
	$end = $start + $size if $end - $start > $size;
	my ($n) = $end - $start;

	printf "0x%08x  ", int ($ofs / $per_line) * $per_line;

	# Hex version.
	print "   " x $start;
	for my $i ($start...$end - 1) {
	    printf "%02x", ord (substr ($data, $i - $start, 1));
	    print $i == $per_line / 2 - 1 ? '-' : ' ';
	}
	print "   " x ($per_line - $end);

	# Character version.
	my ($esc_data) = substr ($data, 0, $n);
	$esc_data =~ s/[^[:print:]]/./g;
	print "|", " " x $start, $esc_data, " " x ($per_line - $end), "|";

	print "\n";

	$data = substr ($data, $n);
	$ofs += $n;
    }
}

# Utilities.

sub fail {
    finish ("FAIL", @_);
}

sub pass {
    finish ("PASS", @_);
}

sub finish {
    my ($verdict, @messages) = @_;

    seek ($msg_file, 0, 0);
    push (@messages, <$msg_file>);
    close ($msg_file);
    chomp (@messages);

    my ($result_fn) = "$test.result";
    open (RESULT, '>', $result_fn) or die "$result_fn: create: $!\n";
    print RESULT "$verdict\n";
    print RESULT "$_\n" foreach @messages;
    close (RESULT);

    if ($verdict eq 'PASS') {
	print STDOUT "pass $test\n";
    } else {
	print STDOUT "FAIL $test\n";
    }
    print STDOUT "$_\n" foreach @messages;

    exit 0;
}

sub read_text_file {
    my ($file_name) = @_;
    open (FILE, '<', $file_name) or die "$file_name: open: $!\n";
    my (@content) = <FILE>;
    chomp (@content);
    close (FILE);
    return @content;
}

1;
