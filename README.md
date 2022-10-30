# System Updater v1.9b (for gentoo)

### Usage:
```
/usr/sbin/sysup <options>

Options:
    --nosync, -ns      - skip sync phase
    --noportage, -np   - skip portage update phase
    --norebuild, -nr   - skip rebuild phase
    --noclean, -nc     - skip clean phase
    --noask, -na       - don't ask
    --quiet, -q        - hide output

    --fixperl, -fp     - fix perl
    --resume, -r       - resume
    --skipfirst, -sf   - skip first

    # skip all except given phases
    --sync, -s         - sync phase
    --portage, -p      - portage update phase
    --rebuild, -rb     - rebuild phase
    --clean, -c        - clean phase
```

### Installation:
```make install```

### Configuration files:
```
/etc/sysup/sysup.conf (Configuration)
/etc/sysup/phase.conf (Update commands)
/etc/sysup/functions.inc (internal functions)
/etc/sysup/pre.sh (optinal preprocessing script)
/etc/sysup/post.sh (optinal postprocessing script)
```

