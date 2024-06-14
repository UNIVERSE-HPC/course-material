---
name: "Using Docker with Jekyll"
teaching: 20
exercises: 0
dependsOn: [
  technology_and_tooling.docker.docker-image-examples
]
tags: [docker]
attribution: 
    - citation: >
        D. M. Eyers, S. L. R. Stevens, A. Turner, C. Koch and J. Cohen. "Reproducible computational environments using containers: Introduction to Docker".
        Version 2020.09a (4a93bd67aa), September 2020. Carpentries Incubator. 
      url: https://github.com/carpentries-incubator/docker-introduction
      image: https://carpentries-incubator.github.io/docker-introduction/assets/img/incubator-logo-blue.svg
      license: CC-BY-4.0
---

As previously mentioned earlier in the lesson, containers can be helpful for
using software that can be difficult to install.  An example is the software
that generates this lesson website.  The website for this lesson is generated mechanically,
based on a set of files that specify the configuration of the site, its presentation template,
and the content to go on this page.  When working on updates to this lesson,
you might want to preview those changes using a local copy of the website.
This requires installing Jekyll and dependencies such as Ruby and Gemfiles to your local computer
which can be difficult to achieve given complexities such as needing to match specific versions of the software components. Instead you could use Docker and a pre-built Jekyll container image.

First we need to get a copy of the website source to work with on your computer.
In your shell window, in your `docker-intro` create a new directory `build-website` and `cd` into it. We will be expanding a ZIP file into this directory later.

Now open a web browser window and:

1. Navigate to the [GitHub repository](https://github.com/carpentries-incubator/docker-introduction) that contains the files for this session;
2. Click the green "Clone or download" button on the right-hand side of the page;
3. Click "Download ZIP".
4. The downloaded ZIP file should contain one directory named `docker-introduction-gh-pages`.
5. Move the `docker-introduction-gh-pages` folder into the `build-website` folder you created above.

:::callout

## There are many ways to work with ZIP files

Note that the last two steps can be achieved using a Mac or Windows graphical user interface. There are also ways to effect expanding the ZIP archive on the command line, for example, on my Mac I can achieve the effect of those last two steps through running the command `unzip ~/Downloads/docker-introduction-gh-pages.zip`.
:::

In your shell window, if you `cd` into the `docker-introduction-gh-pages` folder and list the files, you should see something similar to what I see:

~~~bash
cd docker-introduction-gh-pages
ls
~~~

~~~text
AUTHORS   _episodes  code
CITATION  _episodes_rmd  data
CODE_OF_CONDUCT.md _extras   fig
CONTRIBUTING.md  _includes  files
LICENSE.md  _layouts  index.md
Makefile  aio.md   reference.md
README.md  assets   setup.md
_config.yml  bin
~~~

You can now request that a container is created that will compile the files in
this set into the lesson website, and will run a simple webserver to allow you
to view your version of the website locally. Note that this command will be long
and fiddly to type, so you probably want to copy-and-paste it into your shell
window. This command will continue to (re-)generate and serve up your version of
the lesson website, so you will not get your shell prompt back until you type
<kbd>control</kbd>+<kbd>c</kbd>. This will stop the webserver, since it cleans
away the container.

For macOS, Linux and PowerShell:

~~~bash
docker run --rm -it --mount type=bind,source=${PWD},target=/srv/jekyll -p 127.0.0.1:4000:4000 jekyll/jekyll:3 jekyll serve
~~~

When I ran the macOS command, the output was as follows:

~~~text
Unable to find image 'jekyll/jekyll:3' locally
3: Pulling from jekyll/jekyll
9d48c3bd43c5: Pull complete 
9ce9598067e7: Pull complete 
278f4c997324: Pull complete 
bfca09e5fd9a: Pull complete 
2612f15b9d22: Pull complete 
322c093d5418: Pull complete 
Digest: sha256:9521c8aae4739fcbc7137ead19f91841b833d671542f13e91ca40280e88d6e34
Status: Downloaded newer image for jekyll/jekyll:3

...output trimmed...

ruby 2.6.3p62 (2019-04-16 revision 67580) [x86_64-linux-musl]
Configuration file: /srv/jekyll/_config.yml
To use retry middleware with Faraday v2.0+, install `faraday-retry` gem
            Source: /srv/jekyll
       Destination: /srv/jekyll/_site
 Incremental build: disabled. Enable with --incremental
      Generating... 
      Remote Theme: Using theme carpentries/carpentries-theme
                    done in 7.007 seconds.
 Auto-regeneration: enabled for '/srv/jekyll'
    Server address: http://0.0.0.0:4000
  Server running... press ctrl-c to stop.
~~~

In the preceding output, you see Docker downloading the container image for
Jekyll, which is a tool for building websites from specification files such as
those used for this lesson. The line `jekyll serve` indicates a command that
runs within the Docker container instance. The output below that is from the
Jekyll tool itself, highlighting that the website has been built, and indicating
that there is a server running.

Open a web browser window and visit the address [http://localhost:4000/](http://localhost:4000/). You
should see a site that looks very similar to that at
[https://carpentries-incubator.github.io/docker-introduction/](https://carpentries-incubator.github.io/docker-introduction/).

Using a new shell window, or using your laptop's GUI, locate the file `index.md`
within the `docker-introduction-gh-pages` directory, and open it in your
preferred editor program.

Near the top of this file you should see the description starting "This session
aims to introduce the use of Docker containers with the goal of using them to
effect reproducible computational environments." Make a change to this message,
and save the file.

If you reload your web browser, the change that you just made should be visible.
This is because the Jekyll container saw that you changed the `index.md` file,
and regenerated the website.

You can stop the Jekyll container by clicking in its terminal window and typing
`Ctrl-C`

You have now achieved using a reproducible computational environment to
reproduce a lesson about reproducible computing environments.
