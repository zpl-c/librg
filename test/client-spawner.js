const readline  = require('readline');
const { spawn } = require('child_process');
let clients = [];

const proc_path = './build/client';

var rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
});

console.log('start NUMBER - spawns client processes')
console.log('stop - stops all proccesses')

rl.on('line', function (cmd) {
    if (cmd.startsWith('start')) {
        const amount = parseInt(cmd.split(' ')[1]) || 10;

        for (let i = 0; i < amount; i++) {
            console.log('spawning client #' + i);
            let bat = spawn(proc_path);

            clients.push(bat);

            // sleep
            var stop = new Date().getTime();
            while(new Date().getTime() < stop + 100) {}
        }
    }


    if (cmd.startsWith('stop') || cmd == 'stop') {
        console.log('stopping all clients');
        clients.map(proc => proc.kill('SIGHUP'));
        clients = [];
    }
});
