EXEEXT = 
PROGRAM = ../hexdump$(EXEEXT)

# Append test names here, separated by a space. If the line goes beyond HERE !
# add a \ to the end of the line and continue on the next line.
TESTS = basic verbose

# Specify the arguments for each test case here
basic_ARGS = 
verbose_ARGS = -v

#####################################################################
# Make no edits below this line unless you know what you are doing! #
#####################################################################

$(foreach test,$(TESTS),$(eval $(test).test: TEST = $(test)))

ALLTESTS = $(addsuffix .test,$(TESTS))
OUTPUTS = $(addprefix tests/,$(addsuffix .output,$(TESTS)))
ERRORS = /dev/null

# The following 4 lines build the command line to automatically
# run the executable, redirecting output to appropriate files.
# For a test case like tests/verbose, the command line would be:
#   ../hexdump -v | tee tests/verbose.output
TESTCMD = $(PROGRAM)
TESTCMD += $(if $($(TEST)_ARGS),$($(TEST)_ARGS),$(*F))

all.test: $(ALLTESTS)

%.test:
	@if [ $@ != "all.test" ] ; then 							\
		if [ ! -f $(PROGRAM) ] ; then 							\
			echo "Executable $(PROGRAM) does not exist. Did you run make first?";	\
		else 										\
			echo "Running '$(TESTCMD)':"; 						\
			echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"; 	\
			$(TESTCMD) | tee $(TEST).output; 					\
			echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"; 	\
			echo "Output copied to tests/$(TEST).output for your reference"; 	\
			echo ""; 								\
		fi; 										\
	fi
