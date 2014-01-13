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

THIS IS PATCHED NOW WITH evasi0n7 0.3 yo you don't need this patch anymore.
The patch is available on http://repo.tihmstar.org simply install it via Cydia ;)

btw: @evad3rs you are such assholes not giving me any credit for finding this,
you guys haven't me even mentioned in the changelog!
i hoped to get some followers by publishing this problem and improving people's
security but now i see what really happens in this scene and i'm not sure
if i will publish future bugs/security issues!


For more information please check out my 2 Videos about this:

English: https://www.youtube.com/watch?v=GJsWT-kQ9MY

Deutsch: not coming, because it's patched in evasi0n7 0.3


Thanks a lot to DarkMalloc allowing me to use parts of his breakout JB code :D
