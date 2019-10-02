# idcpass

idcpass is a simple password manager.  Its only feature is the ability to type
passwords for you.  This hooks into X11, so it will work with any application,
including typing into virtual machines, and does not use any of the
clipboards.

```
rharwood@seton:~/idcpass.git/master$ make -s
rharwood@seton:~/idcpass.git/master$ ./idcpass --help
Usage: ./idcpass init|add|lookup|x
    init: create a new store
    add: create a new entry in the store
    delete: remove an entry from the store
    lookup: text lookup of an entry from the store
    list: list services present in the store
    x: lookup an entry with dmenu, then type result using X11

rharwood@seton:~/idcpass.git/master$ 
                        
```

