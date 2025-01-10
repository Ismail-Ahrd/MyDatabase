const { spawnSync } = require('child_process');
const fs = require('fs');

describe('database', () => {
  beforeAll(() => {
    const dbFilePath = 'test.db';
    if (fs.existsSync(dbFilePath)) {
      fs.unlinkSync(dbFilePath);
    }
  });

  function runScript(commands) {
    const process = spawnSync('../build/part10', ['test.db'], { input: commands.join('\n'), encoding: 'utf-8' });

    // The stdout will contain the output from the program
    const rawOutput = process.stdout;
    // Split the output by newlines and return the result
    return rawOutput.split('\n');
  }

  it('allows printing out the structure of a 3-leaf-node btree', () => {
    const script = Array.from({ length: 14 }, (_, i) => `insert ${i + 1} user${i + 1} person${i + 1}@example.com`);
    script.push('.btree');
    script.push('insert 15 user15 person15@example.com');
    script.push('.exit');

    const result = runScript(script);
    expect(result.slice(14)).toEqual([
      'db > Tree:',
      '- internal (size 1)',
      '  - leaf (size 7)',
      '    - 1',
      '    - 2',
      '    - 3',
      '    - 4',
      '    - 5',
      '    - 6',
      '    - 7',
      '  - key 7',
      '  - leaf (size 7)',
      '    - 8',
      '    - 9',
      '    - 10',
      '    - 11',
      '    - 12',
      '    - 13',
      '    - 14',
      'db > Need to implement searching an internal node',
      ''
    ]);
  });
});
