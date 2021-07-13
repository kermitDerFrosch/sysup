# System Updater v1.0 (for gentoo)

### Usage:
```
sysup [-nc] [-ns] [-np] [-nr] [-fp] [-r] [-q] [-na]

Options:
    --noclean, -nc     - skip clean phase
    --nosync, -ns      - skip sync phase
    --noportage, -np   - skip portage update phase
    --norebuild, -nr   - skip rebuild phase
    --noask, -na       - don't ask
    --quiet, -q        - hide output

    --fixperl, -fp     - fix perl
    --resume, -r       - resume
```

### Installation:
```make install```

### Configuration files:
```
/etc/sysup/sysup.conf (Configuration)
/etc/sysup/phase.conf (Update commands)
```

