---
title: "Exploring and Running Containers"
teaching: 20
exercises: 10
dependsOn: [
  technology_and_tooling.docker.meet-docker
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

:::callout

## Reminder of terminology: container images and containers

Recall that a *container image* is the template from which particular instances of *containers* will be created.
:::

Let's explore our first Docker container. The Docker team provides a simple container
image online called `hello-world`. We'll start with that one.

## Downloading Docker images

The `docker image` command is used to interact with Docker container images.
You can find out what container images you have on your computer by using the following command ("ls" is short for "list"):

~~~bash
docker image ls
~~~

If you've just
installed Docker, you won't see any container images listed.

To get a copy of the `hello-world` Docker container image from the internet, run this command:

~~~bash
docker image pull hello-world
~~~

You should see output like this:

~~~text
Using default tag: latest
latest: Pulling from library/hello-world
1b930d010525: Pull complete
Digest: sha256:f9dfddf63636d84ef479d645ab5885156ae030f611a56f3a7ac7f2fdd86d7e4e
Status: Downloaded newer image for hello-world:latest
docker.io/library/hello-world:latest
~~~

:::callout

## Docker Hub

Where did the `hello-world` container image come from? It came from the Docker Hub
website, which is a place to share Docker container images with other people. More on that
in a later episode.
:::

::::challenge{id=check-images title="Check on Your Images"}

What command would you use to see if the `hello-world` Docker container image had downloaded
successfully and was on your computer?
Give it a try before checking the solution.

:::solution

To see if the `hello-world` container image is now on your computer, run:

~~~bash
docker image ls
~~~

:::
::::

Note that the downloaded `hello-world` container image is not in the folder where you are in the terminal! (Run
`ls` by itself to check.) The container image is not a file like our normal programs and documents;
Docker stores it in a specific location that isn't commonly accessed, so it's necessary
to use the special `docker image` command to see what Docker container images you have on your
computer.

## Running the `hello-world` container

To create and run containers from named Docker container images you use the `docker container run` command. Try the following `docker container run` invocation. Note that it does not matter what your current working directory is.

~~~bash
docker container run hello-world
~~~

~~~text
Hello from Docker!
This message shows that your installation appears to be working correctly.

To generate this message, Docker took the following steps:
 1. The Docker client contacted the Docker daemon.
 2. The Docker daemon pulled the "hello-world" image from the Docker Hub.
    (amd64)
 3. The Docker daemon created a new container from that image which runs the
    executable that produces the output you are currently reading.
 4. The Docker daemon streamed that output to the Docker client, which sent it
    to your terminal.

To try something more ambitious, you can run an Ubuntu container with:
 $ docker run -it ubuntu bash

Share images, automate workflows, and more with a free Docker ID:
 https://hub.docker.com/

For more examples and ideas, visit:
 https://docs.docker.com/get-started/
~~~

What just happened? When we use the `docker container run` command, Docker does three things:

| 1. Starts a Running Container | 2. Performs Default Action | 3. Shuts Down the Container |
| --------------------|-----------------|----------------|
| Starts a running container, based on the container image. Think of this as the "alive" or "inflated" version of the container -- it's actually doing something. | If the container has a default action set, it will perform that default action. This could be as simple as printing a message (as above) or running a whole analysis pipeline! | Once the default action is complete, the container stops running (or exits). The container image is still there, but nothing is actively running. |

The `hello-world` container is set up to run an action by default --
namely to print this message.

:::callout

## Using `docker container run` to get the image

We could have skipped the `docker image pull` step; if you use the `docker container run`
command and you don't already have a copy of the Docker container image, Docker will
automatically pull the container image first and then run it.
:::

## Running a container with a chosen command

But what if we wanted to do something different with the container? The output
just gave us a suggestion of what to do -- let's use a different Docker container image
to explore what else we can do with the `docker container run` command. The suggestion above
is to use `ubuntu`, but we're going to run a different type of Linux, `alpine`
instead because it's quicker to download.

:::challenge{id=run-alpine title="Run the Alpine Docker container"}

Try downloading the `alpine` container image and using it to run a container. You can do it in
two steps, or one. What are they?
:::

What happened when you ran the Alpine Docker container?

~~~bash
docker container run alpine
~~~

If you have never used the `alpine` Docker container image on your computer, Docker probably printed a
message that it couldn't find the container image and had to download it.
If you used the `alpine` container image before, the command will probably show no output. That's
because this particular container is designed for you to provide commands yourself. Try running
this instead:

~~~bash
docker container run alpine cat /etc/os-release
~~~

You should see the output of the `cat /etc/os-release` command, which prints out
the version of Alpine Linux that this container is using and a few additional bits of information.

::::challenge{id=hello-world2 title="Hello World, Part 2"}

Can you run a copy of the `alpine` container and make it print a "hello world" message?

Give it a try before checking the solution.

:::solution

Use the same command as above, but with the `echo` command to print a message.

~~~bash
docker container run alpine echo 'Hello World'
~~~

:::
::::

So here, we see another option -- we can provide commands at the end of the `docker container run`
command and they will execute inside the running container.

## Running containers interactively

In all the examples above, Docker has started the container, run a command, and then
immediately stopped the container. But what if we wanted to keep the container
running so we could log into it and test drive more commands? The way to
do this is by adding the interactive flags `-i` and `-t` (usually combined as `-it`)
to the `docker container run` command and provide a shell (`bash`,`sh`, etc.)
as our command. The `alpine` Docker container image doesn't include `bash` so we need
to use `sh`.

~~~bash
docker container run -it alpine sh
~~~

:::callout

## Technically

Technically, the interactive flag is just `-i` -- the extra `-t` (combined
as `-it` above) is the "pseudo-TTY" option, a fancy term that means a text interface.
This allows you to connect to a shell, like `sh`, using a command line. Since you usually
want to have a command line when running interactively, it makes sense to use the two together.
:::

Your prompt should change significantly to look like this:

~~~bash
/ #
~~~

That's because you're now inside the running container! Try these commands:

* `pwd`
* `ls`
* `whoami`
* `echo $PATH`
* `cat /etc/os-release`

All of these are being run from inside the running container, so you'll get information
about the container itself, instead of your computer. To finish using the container,
type `exit`.

~~~bash
/ # exit
~~~

::::challenge{id=practice-makes-perfect title="Practice Makes Perfect"}

Can you find out the version of Ubuntu installed on the `ubuntu` container image?
(Hint: You can use the same command as used to find the version of alpine.)

Can you also find the `apt-get` program? What does it do? (Hint: try passing `--help`
to almost any command will give you more information.)

:::solution

## Solution 1 -- Interactive

Run an interactive busybox container -- you can use `docker image pull` first, or just
run it with this command:

~~~bash
docker container run -it ubuntu sh
~~~

OR you can get the bash shell instead

~~~bash
docker container run -it ubuntu bash
~~~

Then try, running these commands

~~~bash
/# cat /etc/os-release
/# apt-get --help
~~~

Exit when you're done.

~~~bash
/# exit
~~~

## Solution 2 -- Run commands

Run a ubuntu container, first with a command to read out the Linux version:

~~~bash
docker container run ubuntu cat /etc/os-release
~~~

Then run a container with a command to print out the apt-get help:

~~~bash
docker container run ubuntu apt-get --help
~~~

:::
::::

:::callout

## Even More Options

There are many more options, besides `-it` that can be used with the `docker container run`
command!  A few of them will be covered in [later episodes](advanced-containers)
and we'll share two more common ones here:

* `--rm`: this option guarantees that any running container is completely
removed from your computer after the container is stopped. Without this option,
Docker actually keeps the "stopped" container around, which you'll see in a later
episode. Note that this option doesn't impact the *container images* that you've pulled,
just running instances of containers.

* `--name=`: By default, Docker assigns a random name and ID number to each container
instance that you run on your computer. If you want to be able to more easily refer
to a specific running container, you can assign it a name using this option.

:::

## Conclusion

So far, we've seen how to download Docker container images, use them to run commands inside
running containers, and even how to explore a running container from the inside.
Next, we'll take a closer look at all the different kinds of Docker container images that are out there.

## Key Points

* The `docker image pull` command downloads Docker container images from the internet.
* The `docker image ls` command lists Docker container images that are (now) on your computer.
* The `docker container run` command creates running containers from container images and can run commands inside them.
* When using the docker container run command, a container can run a default action (if it has one), a user specified action, or a shell to be used interactively.
