# System Updater v1.0 (for gentoo)

### Usage:
```
sysup [-nc] [-ns] [-np] [-nr] [-fp] [-q]

Options:
    --noclean, -nc     - skip clean phase
    --nosync, -ns      - skip sync phase
    --noportage, -np   - skip portage update phase
    --norebuild, -nr   - skip rebuild phase
    --quiet, -q        - hide output

    --fixperl, -fp     - fix perl
```

### Installation:
```make install```

### Configuration files:
```
/etc/sysup/sysup.conf (Configuration)
/etc/sysup/phase.conf (Update commands)
```

