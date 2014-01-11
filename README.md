exVasi0n is my demonstartion proof of concept code
which shows a security issue with the evasi0n7 jailbreak.

The problem is that the evasi0n7 untether binary checks on EVERY REBOOT if
a the file /var/mobile/Media/evasi0n-install/Cydia.tar exists.
If so the tar gets extracted to /
Being able to create the evasi0n-install dir and push your own Cydia.tar via 
the default sandboxed afc (not afc2!) this allows everyone with physical access
to your phone to put any Payload, any file, any LaunchDaemon 
basically anything he wants to your phone, even without you knowing about it.

Because waiting for evad3rs to fix this would mean being a while unprotected,
uroboro and i created a hotpatch for this preventing files being written to
evasi0n-install dir.

This mylittlesecret.tar will install openssh + openssl on your device.
Feel free to try this out yourself ;)

The patch is available on http://repo.tihmstar.org simply install it via Cydia ;)


For more information please check out my 2 Videos about this:

English: https://www.youtube.com/watch?v=GJsWT-kQ9MY

Deutsch: coming soon


Thanks a lot to DarkMalloc allowing me to use parts of his breakout JB code :D
