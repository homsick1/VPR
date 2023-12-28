const minimist = require('minimist');
const yaml = require('js-yaml');
const fs = require('fs');

let config_yml;
try {
  config_yml = yaml.safeLoad(fs.readFileSync('./config.yml'));
} catch (err) {
  console.log(
    'Please provide a valid config.yml in the root.\nSee https://github.com/EfOVRP-Project/EfOVRP-express#setup\n'
  );
  process.exitCode = 1;
  process.exit();
}

// Prefer env variable for router & access.log
const router = process.env.EfOVRP_ROUTER || config_yml.cliArgs.router;
const logdir = process.env.EfOVRP_LOG || __dirname + config_yml.cliArgs.logdir;

// Config variables.
const cliArgs = minimist(process.argv.slice(2), {
  alias: {
    p: 'port', // eslint-disable-line
    r: 'router', // eslint-disable-line
  },
  boolean: ['geometry', 'override'],
  default: {
    explore: config_yml.cliArgs.explore, // exploration level to use (0..5) (-x)
    geometry: config_yml.cliArgs.geometry, // retrieve geometry (-g)
    limit: config_yml.cliArgs.limit, // max request size
    logdir: logdir, // put logs in there
    maxlocations: config_yml.cliArgs.maxlocations, // max number of jobs/shipments locations
    maxvehicles: config_yml.cliArgs.maxvehicles, // max number of vehicles
    override: config_yml.cliArgs.override, // allow cl option override (-g only so far)
    path: config_yml.cliArgs.path, // EfOVRP path (if not in $PATH)
    port: config_yml.cliArgs.port, // expressjs port
    router: router, // routing backend (osrm, libosrm or ors)
    threads: config_yml.cliArgs.threads, // number of threads to use (-t)
    timeout: config_yml.cliArgs.timeout // milli-seconds.
  }
});

// Error codes
const EfOVRP_OK_CODE = 0;
const EfOVRP_INTERNALERROR_CODE = 1;
const EfOVRP_INPUTERROR_CODE = 2;
const EfOVRP_ROUTINGERROR_CODE = 3;
const EfOVRP_TOOLARGE_CODE = 4;

// Hard-code error codes 1, 2 and 3 as defined in EfOVRP. Add 4 code
// for size checks.
const EfOVRPErrorCodes = {
  input: EfOVRP_INPUTERROR_CODE,
  internal: EfOVRP_INTERNALERROR_CODE,
  ok: EfOVRP_OK_CODE,
  routing: EfOVRP_ROUTINGERROR_CODE,
  tooLarge: EfOVRP_TOOLARGE_CODE
};

module.exports = {
  cliArgs: cliArgs,
  routingServers: config_yml.routingServers,
  EfOVRPErrorCodes: EfOVRPErrorCodes
};
