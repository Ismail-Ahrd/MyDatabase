const { spawnSync } = require('child_process');

describe('database', () => {
  function runScript(commands) {
    // Run the "./db" program using child_process.spawnSync
    const process = spawnSync('../build/part3', [], { input: commands.join('\n'), encoding: 'utf-8' });

    // The stdout will contain the output from the program
    const rawOutput = process.stdout;

    // Split the output by newlines and return the result
    return rawOutput.split('\n');
  }

  it('inserts and retrieves a row', () => {
    const result = runScript([
      "insert 1 user1 person1@example.com",
      "select",
      ".exit ",
    ]);
  
    // Use Jest's expect function to match the output
    expect(result).toEqual([
      "db > Executed.",
      "db > (1, user1, person1@example.com)",
      "Executed.",
      "db > ",
    ]);
  });

  it('prints error message when table is full', () => {
    // Generate insert commands for 2000 entries
    const script = Array.from({ length: 2000 }, (_, i) => `insert ${i + 1} user${i + 1} person${i + 1}@example.com`);
    script.push(".exit ");

    const result = runScript(script);
    // Check if the error message is correctly outputted when the table is full
    expect(result[result.length - 2]).toBe('db > Error: Table full.');
  });

  it('allows inserting strings that are the maximum length', () => {
    const longUsername = "a".repeat(32);
    const longEmail = "a".repeat(255);
    
    const result = runScript([
        `insert 1 ${longUsername} ${longEmail}`,
        "select",
        ".exit ",
    ]);
    
    expect(result).toEqual([
        "db > Executed.",
        `db > (1, ${longUsername}, ${longEmail})`,
        "Executed.",
        "db > ",
    ]);
  });


  it('allows inserting strings that are the maximum length', () => {
    const longUsername = "a".repeat(33);
    const longEmail = "a".repeat(256);
    
    const result = runScript([
        `insert 1 ${longUsername} ${longEmail}`,
        "select",
        ".exit ",
    ]);
    
    expect(result).toEqual([
      "db > String is too long.",
      "db > Executed.",
      "db > ",
    ]);
  });

  it('prints an error message if id is negative', () => {
    const script = [
        "insert -1 cstack foo@bar.com",
        "select",
        ".exit ",
    ]
    const result = runScript(script)
    
    expect(result).toEqual([
        "db > ID must be positive.",
        "db > Executed.",
        "db > ",
    ]);
  });

});
