collection_templates.h: src/*.h build.sh
	./build.sh

docs: collection_templates.h
	cldoc generate -- --language c --output docs/ src/*

serve_docs:
	cldoc serve -- docs/

tests: collection_templates.h
	make -C test
