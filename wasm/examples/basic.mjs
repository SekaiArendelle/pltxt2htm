import assert from "node:assert/strict";
import { resolve } from "node:path";
import { pathToFileURL } from "node:url";

const { default: createModule } = await import(pathToFileURL(resolve(process.argv[2])).href);
const module = await createModule();
const parse = module.cwrap("common_parser", "number", ["string"]);
const result = module.UTF8ToString(parse("<b>test"));
assert.equal(result, "<strong>test</strong>");
console.log(result);
