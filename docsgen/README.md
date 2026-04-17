# Doxygen docs

Use Docker to generate docs from repository root:

```sh
docker build -t pltxt2htm-docs -f docsgen/Dockerfile .
mkdir docs
docker run --rm -v "docs/:/src/docsgen/docs" pltxt2htm-docs
```

Then open:

```text
docsgen/build/doxygen/html/index.html
```
