const readline  = require('readline');
const { spawn } = require('child_process');
let clients = [];

const proc_path = './build/client';

var rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
});

console.log('spawn NUMBER - spawns client processes')
console.log('stop - stops all proccesses')

rl.on('line', function (cmd) {
    if (cmd.startsWith('start')) {
        const amount = parseInt(cmd.split(' ')[1]) || 10;

        for (let i = 0; i < amount; i++) {
            console.log('spawning client #' + i);
            let bat = spawn(proc_path);

            bat.stderr.on('data', (data) => {
                console.log('client #' + i, data.toString());
            });

            bat.on('exit', (code) => {
              console.log('client #' + i, `Child exited with code ${code}`);
            });

            clients.push(bat);
        }
    }


    if (cmd.startsWith('stop') || cmd == 'stop') {
        console.log('stopping all clients');
        clients.map(proc => proc.kill('SIGHUP'));
        clients = [];
    }
});
