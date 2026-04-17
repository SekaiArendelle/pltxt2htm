# Doxygen docs

Use Docker to generate docs from repository root:

```sh
docker build -f docsgen/Dockerfile -t pltxt2htm-docs .
docker create --name pltxt2htm-docs-container pltxt2htm-docs
docker cp pltxt2htm-docs-container:/src/docsgen/docs ./docs
```

Then open:

```text
docsgen/build/doxygen/html/index.html
```
