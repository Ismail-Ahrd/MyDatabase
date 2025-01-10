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
    const process = spawnSync('../build/part12', ['test.db'], { input: commands.join('\n'), encoding: 'utf-8' });

    // The stdout will contain the output from the program
    const rawOutput = process.stdout;
    // Split the output by newlines and return the result
    return rawOutput.split('\n');
  }

  it('prints all rows in a multi-level tree', () => {
    const script = Array.from({ length: 15 }, (_, i) => 
      `insert ${i + 1} user${i + 1} person${i + 1}@example.com`
    );
    script.push('select');
    script.push('.exit ');

    const result = runScript(script);

    expect(result.slice(15)).toEqual([
      'db > (1, user1, person1@example.com)',
      '(2, user2, person2@example.com)',
      '(3, user3, person3@example.com)',
      '(4, user4, person4@example.com)',
      '(5, user5, person5@example.com)',
      '(6, user6, person6@example.com)',
      '(7, user7, person7@example.com)',
      '(8, user8, person8@example.com)',
      '(9, user9, person9@example.com)',
      '(10, user10, person10@example.com)',
      '(11, user11, person11@example.com)',
      '(12, user12, person12@example.com)',
      '(13, user13, person13@example.com)',
      '(14, user14, person14@example.com)',
      '(15, user15, person15@example.com)',
      'Executed.',
      'db > ',
    ]);
  });
});
