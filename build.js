var spawnSync = require('child_process').spawnSync;
var os = require('os');

function exec(cmd) {
    const { status } = spawnSync(cmd, {
        shell: true,
        stdio: 'inherit',
    });
    process.exit(status);
}

exec("npx cmake-js rebuild --config Release"); 
