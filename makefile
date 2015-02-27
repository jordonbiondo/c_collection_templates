collection_templates.h: src/*.h
	./build.sh

docs: src/*.h
	cldoc generate -- --language c --output docs/ src/*

serve_docs:
	cldoc serve -- docs/
