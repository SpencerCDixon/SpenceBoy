var fs = require('fs');

console.log('--> Generating op codes...');

const json = require('./op_codes.json')['unprefixed'];
const keys = Object.keys(json);

let codes = [];

keys.forEach(key => {
  const operands = json[key].operands.map(v => v.name);
  const name = [json[key].mnemonic, ...operands].join('_');
  const cycles = json[key].cycles.reduce((acc, val) => (acc += val), 0);

  codes.push({
    op_code: key,
    name,
    cycles,
  });
});

codes.sort((o1, o2) => (o1.name > o2.name ? 1 : -1));
codes.forEach(({ op_code, name, cycles }) => {
  const str = `__ENUMERATE(${op_code}, ${name}, ${cycles})`;
  console.log(str);

  // const str = `- [ ] ${name} (${op_code})`;
  // console.log(str);
});

console.log(`--> ${codes.length} codes generated`);
