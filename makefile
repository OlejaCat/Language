all: language full_processor

full_processor:
	@$(MAKE) -C Processor

language:
	@$(MAKE) -C Language

clean:
	@$(MAKE) -C Processor clean
	@$(MAKE) -C Language clean

.PHONY: language full_processor
