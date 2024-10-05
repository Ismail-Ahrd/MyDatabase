const { spawnSync } = require('child_process');

describe('database', () => {
  function runScript(commands) {
    const process = spawnSync('../build/part5', ['test.db'], { input: commands.join('\n'), encoding: 'utf-8' });

    // The stdout will contain the output from the program
    const rawOutput = process.stdout;
    // Split the output by newlines and return the result
    return rawOutput.split('\n');
  }

  it('keeps data after closing connection', () => {
    const result1 = runScript([
        "insert 1 user1 person1@example.com",
        ".exit ",
    ]);
    
    expect(result1).toEqual([
        "db > Executed.",
        "db > ",
    ]);

    const result2 = runScript([
        "select",
        ".exit ",
    ]);
    
    expect(result2).toEqual([
        "db > (1, user1, person1@example.com)",
        "Executed.",
        "db > ",
    ]);
  });

  

});
