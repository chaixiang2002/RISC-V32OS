SECTIONS = \
	src \
	00-boot \
	01-hello \
	02-memanagement \
	034-multitask \
	05-traps \
	06-interrupts \
	07-hwtimer \
	08-preemptive \
	09-lock \
	1011swtimer_syscall \

.DEFAULT_GOAL := all
all :
	@echo "begin compile ALL exercises for assembly samples ......................."
	for dir in $(SECTIONS); do $(MAKE) -C $$dir || exit "$$?"; done
	@echo "compile ALL exercises finished successfully! ......"

.PHONY : clean
clean:
	for dir in $(SECTIONS); do $(MAKE) -C $$dir clean || exit "$$?"; done
