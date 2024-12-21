-----

<p align="center">
	<img width="350" src="https://github.com/auth-xyz/assets/blob/main/logos/chunky.png?raw=true" alt="chunky logo">
</p>

-----

### servy 

**servy** is another tool I wrote to deal with the hassle of creating systemd services, it's very barebones, I hope to improve it in the near future so it's very customizable, but in simple words, it creates a systemd service for running scripts for you, how nice is that?

-----

- `What is servy?`:
    `servy` in basic terms, as said previously in the intro creates an systemd service from a template in my assets/ repository, it then edits said template to fit the given arguments, `name`, `script` and `command`, then servy sends those to either .config/systemd/user/ or /etc/systemd/system/ (depends if you run the script with root or not)

- `How is it useful?`:
    It facilitates the creation of services, which for beginner linux users may seem like a nightmare, specially if you have an issue like mine with the laptop's touchscreen(i have a script to disable my laptop's touchscreen 'cause it's broken).

- `Why?`:
    why not?

----

### Setting up

There really isn't any setting up for `servy`, it's just "installing" (building) servy and using it.
* requires [clicky](https://github.com/auth-xyz/clicky)

```bash
$ git clone https://github.com/auth-xyz/servy
$ cd servy/
$ make && make install 
```

----

### Usage 

Hey, unlike most of my projects, servy is pretty straight up to use, only a few arguments and you're good to go
```bash
# Help message
Flags:
  --help, -h       : Display this help message (default: false)

Arguments:
  --command, -c    : command to be used for script execution (required)
  --script, -s     : script to be executed (required)
  --name, -n       : The service's name (optional)


# example usage
servy --script somescript.sh \
      --command /usr/bin/bash \
      --name "someservice"

```

----

### Contributing

If you think my code is shit, it's because it most likely is, and if you have any advice for me on how to improve my code, please reach out on discord <actually.auth>, or create an issue on this repo.


[logo]: https://github.com/auth-xyz/assets/blob/main/logos/chunky.png?raw=true

