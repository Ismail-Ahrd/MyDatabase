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
    const process = spawnSync('../build/part13', ['test.db'], { input: commands.join('\n'), encoding: 'utf-8' });

    // The stdout will contain the output from the program
    const rawOutput = process.stdout;
    // Split the output by newlines and return the result
    return rawOutput.split('\n');
  }

  it('allows printing out the structure of a 4-leaf-node btree', () => {    
    script = [
      "insert 18 user18 person18@example.com",
      "insert 7 user7 person7@example.com",
      "insert 10 user10 person10@example.com",
      "insert 29 user29 person29@example.com",
      "insert 23 user23 person23@example.com",
      "insert 4 user4 person4@example.com",
      "insert 14 user14 person14@example.com",
      "insert 30 user30 person30@example.com",
      "insert 15 user15 person15@example.com",
      "insert 26 user26 person26@example.com",
      "insert 22 user22 person22@example.com",
      "insert 19 user19 person19@example.com",
      "insert 2 user2 person2@example.com",
      "insert 1 user1 person1@example.com",
      "insert 21 user21 person21@example.com",
      "insert 11 user11 person11@example.com",
      "insert 6 user6 person6@example.com",
      "insert 20 user20 person20@example.com",
      "insert 5 user5 person5@example.com",
      "insert 8 user8 person8@example.com",
      "insert 9 user9 person9@example.com",
      "insert 3 user3 person3@example.com",
      "insert 12 user12 person12@example.com",
      "insert 27 user27 person27@example.com",
      "insert 17 user17 person17@example.com",
      "insert 16 user16 person16@example.com",
      "insert 13 user13 person13@example.com",
      "insert 24 user24 person24@example.com",
      "insert 25 user25 person25@example.com",
      "insert 28 user28 person28@example.com",
      ".btree",
      ".exit ",
    ]

    const result = runScript(script);
    //console.log(result.slice(31))

    expect(result.slice(31)).toEqual([
      '- internal (size 3)',
      '  - leaf (size 7)',
      '    - 1',
      '    - 2',
      '    - 3',
      '    - 4',
      '    - 5',
      '    - 6',
      '    - 7',
      '  - key 7',
      '  - leaf (size 8)',
      '    - 8',
      '    - 9',
      '    - 10',
      '    - 11',
      '    - 12',
      '    - 13',
      '    - 14',
      '    - 15',
      '  - key 15',
      '  - leaf (size 7)',
      '    - 16',
      '    - 17',
      '    - 18',
      '    - 19',
      '    - 20',
      '    - 21',
      '    - 22',
      '  - key 22',
      '  - leaf (size 8)',
      '    - 23',
      '    - 24',
      '    - 25',
      '    - 26',
      '    - 27',
      '    - 28',
      '    - 29',
      '    - 30',
      'db > '
  ]);
  });
});
