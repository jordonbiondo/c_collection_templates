src_dir=src

main: collection_templates.h

syntax:
	gcc -fsyntax-only -Wall -pedantic -std=c99 $(src_dir)/*.h

collection_templates.h: syntax src/*.h build.sh
	./build.sh

docs: collection_templates.h
	cldoc generate -- --language c --output docs/ src/*

serve_docs:
	cldoc serve -- docs/

tests: collection_templates.h
	make -C test all

unit_tests: collection_templates.h
	make -C test run_unit_tests

clean:
	rm -f collection_templates.h && make -C test clean
