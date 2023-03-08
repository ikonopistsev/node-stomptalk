var spawnSync = require('child_process').spawnSync;
var os = require('os');

function exec(cmd) {
    const { status } = spawnSync(cmd, {
        shell: true,
        stdio: 'inherit',
    });
    process.exit(status);
}

if (os.type() === 'Linux' || os.type() === 'Darwin') 
   exec("npx cmake-js rebuild --config Release"); 
else if (os.type() === 'Windows_NT') 
   exec("npx cmake-js rebuild --config RelWithDebInfo");
else
   throw new Error("Unsupported OS found: " + os.type());