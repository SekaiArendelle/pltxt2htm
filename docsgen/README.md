# Doxygen docs

Use Docker to generate docs from repository root:

```sh
docker build -f docsgen/Dockerfile -t pltxt2htm-docs .
mkdir docs
docker run --rm -v "docs/:/src/docsgen/docs" pltxt2htm-docs
```

Then open:

```text
docsgen/build/doxygen/html/index.html
```
