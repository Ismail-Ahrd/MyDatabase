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
    const process = spawnSync('../build/part9', ['test.db'], { input: commands.join('\n'), encoding: 'utf-8' });

    // The stdout will contain the output from the program
    const rawOutput = process.stdout;
    // Split the output by newlines and return the result
    return rawOutput.split('\n');
  }

  it('allows printing out the structure of a one-node btree', () => {
    const script = [3, 1, 2].map(i => `insert ${i} user${i} person${i}@example.com`);
    script.push('.btree', '.exit ');
    const result = runScript(script);

    expect(result).toEqual([
      "db > Executed.",
      "db > Executed.",
      "db > Executed.",
      "db > Tree:",
      "leaf (size 3)",
      "  - 0 : 1",
      "  - 1 : 2",
      "  - 2 : 3",
      "db > "
    ]);
  });

  it('prints constants', () => {
    const script = [
      ".constants",
      ".exit ",
    ];
    const result = runScript(script);

    expect(result).toEqual([
      "db > Constants:",
      "ROW_SIZE: 293",
      "COMMON_NODE_HEADER_SIZE: 6",
      "LEAF_NODE_HEADER_SIZE: 10",
      "LEAF_NODE_CELL_SIZE: 297",
      "LEAF_NODE_SPACE_FOR_CELLS: 4086",
      "LEAF_NODE_MAX_CELLS: 13",
      "db > "
    ]);
  });

  it('prints an error message if there is a duplicate id', () => {
    // Ensure the database is removed before this test runs
    const dbFilePath = 'test.db';
    if (fs.existsSync(dbFilePath)) {
      fs.unlinkSync(dbFilePath);
    }

    const script = [
      "insert 1 user1 person1@example.com",
      "insert 1 user1 person1@example.com",
      "select",
      ".exit ",
    ];
    const result = runScript(script);

    expect(result).toEqual([
      "db > Executed.",
      "db > Error: Duplicate key.",
      "db > (1, user1, person1@example.com)",
      "Executed.",
      "db > ",
    ]);
  });
});
