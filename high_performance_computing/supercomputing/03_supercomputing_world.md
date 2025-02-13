---
name: Supercomputing World
dependsOn: [
    high_performance_computing.supercomputing.02_understanding_supercomputing
]
tags: [foundation]
attribution: 
    - citation: >
        "Introduction to HPC" course by EPCC.
        This material was originally developed by David Henty, Manos Farsarakis, Weronika Filinger, James Richings, and Stephen Farr at EPCC under funding from EuroCC.
      url: https://epcced.github.io/Intro-to-HPC/
      image: https://epcced.github.io/Intro-to-HPC/_static/epcc_logo.svg
      license: CC-BY-4.0
---

![Computer circuit board looking like a city](images/bert-b-rhNff6hB41s-unsplash.jpg)
*Image courtesy of [bert b](https://unsplash.com/@bertsz) from [Unsplash](https://unsplash.com)*

## Current Trends and Moore's Law

Over recent decades, computers have become more and more powerful. New computing devices appear on the market at a tremendous rate, and if you like to always have the fastest available personal computer, you need to visit your local computer store very frequently! But how did computers become so powerful, and are there any fundamental limits to how fast a computer can be? To answer these questions, we need to understand what CPUs are made up of.

As mentioned previously, our measure of the performance of a CPU-core is based on the number of floating-point operations it can carry out per second, which in turn depends on the clock speed. CPUs are built from Integrated Circuits that consist of very large numbers of transistors. These transistors are connected by extremely small conducting wires which can carry an electric current. By controlling whether or not an electric current goes through certain conducting lines, we are able to encode information and perform calculations.

Most transistors nowadays are created with silicon, a type of semiconductor. A semiconductor is a material that can act as both a conductor (a material that permits the flow of electrons) and an insulator (that inhibits electron flow), which is exactly the characteristics we want a transistor to have. The maximum physical size of a processor chip is limited to a few square centimetres, so to get a more complicated and powerful processor we need to make the transistors smaller.

In 1965, the co-founder of Fairchild Semiconductor and Intel, Gordon E. Moore, made an observation and forecast. He noticed that manufacturing processes were continually improving to the extent that:

> "The number of transistors that could be placed on an integrated circuit was doubling approximately every two years."

He predicted that this would continue into the future. This observation is named after him, and is called Moore’s law. Although it is really a forecast and not a fundamental law of nature, the prediction has been remarkably accurate for over 50 years.

The first CPU from Intel (the i4004) introduced in 1971 had 2000 transistors, and Intel’s Core i7 CPU introduced in 2012 had 3 billion transistors. This is in excess of a million times more transistors, but is actually in line with what you would expect from the exponential growth of Moore’s law over around 40 years.

It turns out that, as we pack our transistors closer and closer together, every time we double the density of transistors we can double the frequency. So, although Moore’s law is actually a prediction about the density of transistors, for the first four decades it also meant that:

> "Every two years the CPU clock frequency doubled."

We saw clock speeds steadily increasing, finally breaking the GHz barrier (a billion cycles per second) in the early twenty-first century. But then this growth stopped, and clock speeds have remained at a few GHz for more than a decade. So did Moore’s law stop?

The problem is that increasing clock frequency comes at a cost: it takes more power. Above a few GHz, our processors become too power hungry and too hot to use in everyday devices. But Moore’s law continues, so rather than increasing the frequency we put more processors on the same physical chip. We call these CPU-cores, and we now have multicore processors. The below image shows a schematic of a modern processor (Intel’s Core i7) with four CPU-cores (four pinkish rectangles).

![Rendering of Intel Core i7 CPU](images/large_hero_cafacb0d-898b-44b4-9290-5c25c211fc03.jpg)
*Intel's Core i7 A modern quad-core CPU - Intel’s Core i7 © Intel*

So for the past decade, Moore’s law has meant:

> "Every two years, the number of CPU-cores in a processor now doubles."

In the last few years the process of doubling transistors in integrated circuits is showing signs of slowing down. It’s no longer every two years but perhaps every three years, but the overall trend still continues.

The current trend in the supercomputing world is that supercomputers are getting bigger not faster. Since the speed of a single CPU-core cannot be increased any more, having more and more cores working together is the only way to meet our computational requirements.

![Graph of transistor count over time, ](images/Transistor-Count-over-time.png)
*Image courtesy of Max Roser, Hannah Ritchie [OurWorldinData](https://ourworldindata.org/uploads/2020/11/Transistor-Count-over-time.png) ([CC-BY](https://creativecommons.org/licenses/by/4.0/deed.en))*

:::callout{variant="discussion"}
Your next mobile phone will probably have more CPU-cores than your current one. Do you think this is more useful than a faster CPU? Can you see any problems in making use of all these CPU-cores?
:::

---

## How to calculate the world's yearly income?

::::iframe{id="kaltura_player" width="100%" height="400" src="https://cdnapisec.kaltura.com/p/2010292/sp/201029200/embedIframeJs/uiconf_id/32599141/partner_id/2010292?iframeembed=true&playerId=kaltura_player&entry_id=1_tpqo25kw&flashvars[streamerType]=auto&amp;flashvars[localizationCode]=en&amp;flashvars[leadWithHTML5]=true&amp;flashvars[sideBarContainer.plugin]=true&amp;flashvars[sideBarContainer.position]=left&amp;flashvars[sideBarContainer.clickToClose]=true&amp;flashvars[chapters.plugin]=true&amp;flashvars[chapters.layout]=vertical&amp;flashvars[chapters.thumbnailRotator]=false&amp;flashvars[streamSelector.plugin]=true&amp;flashvars[EmbedPlayer.SpinnerTarget]=videoHolder&amp;flashvars[dualScreen.plugin]=true&amp;flashvars[Kaltura.addCrossoriginToIframe]=true&amp;&wid=1_1ms2y7b4" allowfullscreen webkitallowfullscreen mozAllowFullScreen allow="autoplay *; fullscreen *; encrypted-media *" sandbox="allow-downloads allow-forms allow-same-origin allow-scripts allow-top-navigation allow-pointer-lock allow-popups allow-modals allow-orientation-lock allow-popups-to-escape-sandbox allow-presentation allow-top-navigation-by-user-activation" frameborder="0" title="worlds_yearly_income_hd"}
::::

:::solution{title="Transcript"}
0:11 - Here, we’re going to introduce a very simple example, of calculating the world’s yearly income. It’s a bit of a toy example. But, we’re going to imagine that we have a list of the incomes, the salaries, of everybody in the entire world, and we’re going to add them all up to work out what the total income of the world is. Now this is obviously a very simple example, and slightly artificial. But, we’ll actually use it, and come back to it, in a number of contexts. First of all, it is very useful as a specific example of a real calculation, where we can illustrate how much faster calculations have got, through the developments in processor technology over the years.

0:46 - We talk about megahertz, gigahertz, and all kinds of things like this, but if we focus on a specific calculation, it will maybe become more obvious what that actually translates into. Also we’ll come back to this example later on in the course, to see how you might implement it in parallel. How would you use multiple CPU cores to do the calculation faster? So, let’s imagine we have a list. It’s going to be a long list of the salaries of everybody in the world, ordered alphabetically by country and person. So, right at the top of list we have a couple of people, Aadel and Aamir Abdali, who live in Afghanistan. Unfortunately, people in Afghanistan, the average wage is quite low.

1:23 - But they earn just under 1,000 pounds a year each. We carry on down the list, we get a couple of representative people from the UK, Mark and Mary Hensen, a couple who live in the north of England, earning 20,000 or 30,000 pounds each. And there’s me, that’s my salary– oh there seems to be a small error there, you can’t quite see what my salary is, but anyway, I’m on the list as you’d expect. And way down at the bottom– we’re assuming there are seven billion people in the world, which is a reasonable estimate– a couple of brothers from Zimbabwe, Zojj and Zuka Zinyama, who run a successful garage and motor repair business, earning 3,500 and 1,000 pounds each.

1:58 - So what are we going to do to add up all these numbers? What we’re going to do is, we’re going to write a computer programme. Now, as I’ve said before, this isn’t a programming course, we don’t expect you to be a computer programmer. However, this is very simple, and it will serve to illustrate the way that computers work, and allow us to translate these megahertz and gigahertz frequencies we’ve been talking about into actual elapsed time in seconds. So how do we add it up? Well, first of all, we have a running total which we set to zero. And, we start at the top of the list, and we go through the numbers in order.

2:29 - So, we add the income to the total. We go to the next item in the list– the second, the third, the fourth, the fifth. And then we repeat, if we’re not at the end of the list. So if we’re not at the end of the list, but not at the seven billionth entry, we have to go back, add the next income to the total, go to the next item, and then keep repeating, repeating. So we repeat these three steps, three individual steps, we repeat them seven billion times. And once we’ve finished at the end, we can print the total out. So, it’s a very simple prescription, in some kind of pseudo-language of the computer program to add up these incomes.

3:04 - But, the most important point is the core loop, the one that’s executed seven billion times, has three distinct steps in it. And, we’re going to assume that each one of these corresponds to a single instruction issued by the CPU, by the processor. Now, it’s quite a naive assumption, but it’s perfectly OK for our purposes here.
:::

How would you go about calculating the world’s yearly income? Well, it’s simple adding up of numbers but there are many of them… so the real question is: how long would it take?

In this video David describes how to tackle the calculation in serial on his laptop, and in the next step we will discuss how long it might take.

We will use this example in other steps on this course to better illustrate some of the key concepts, so make sure you understand how it works.

---

## Moore's Law in practice

::::iframe{id="kaltura_player" width="100%" height="400" src="https://cdnapisec.kaltura.com/p/2010292/sp/201029200/embedIframeJs/uiconf_id/32599141/partner_id/2010292?iframeembed=true&playerId=kaltura_player&entry_id=1_4zab4d0l&flashvars[streamerType]=auto&amp;flashvars[localizationCode]=en&amp;flashvars[leadWithHTML5]=true&amp;flashvars[sideBarContainer.plugin]=true&amp;flashvars[sideBarContainer.position]=left&amp;flashvars[sideBarContainer.clickToClose]=true&amp;flashvars[chapters.plugin]=true&amp;flashvars[chapters.layout]=vertical&amp;flashvars[chapters.thumbnailRotator]=false&amp;flashvars[streamSelector.plugin]=true&amp;flashvars[EmbedPlayer.SpinnerTarget]=videoHolder&amp;flashvars[dualScreen.plugin]=true&amp;flashvars[Kaltura.addCrossoriginToIframe]=true&amp;&wid=1_1mhy9m0z" allowfullscreen webkitallowfullscreen mozAllowFullScreen allow="autoplay *; fullscreen *; encrypted-media *" sandbox="allow-downloads allow-forms allow-same-origin allow-scripts allow-top-navigation allow-pointer-lock allow-popups allow-modals allow-orientation-lock allow-popups-to-escape-sandbox allow-presentation allow-top-navigation-by-user-activation" frameborder="0" title="Moores_Law_hd"}
::::

:::solution{title="Transcript"}
0:12 - So the question is, how long does this calculation take? So, what I’m going to do is I’m going to do a bit of history. I’m going to look at the history of processors over about the past five decades to see how long it would have taken to do this calculation on a particular processor from that time. I’m going to focus on Intel as a manufacturer. Intel is a very successful manufacturer today of processors– they are very prevalent in desktops and laptops– but there are other designers or manufacturers of processors you may have heard of. ARM, for example, who design a lot of the processors which go into mobile devices like mobile phones, and Nvidia who produce graphics processors.

0:48 - There’s also IBM, International Business Machines, who actually design their own processors, and AMD, who also make their own processors. But I’m going to look at Intel, the main reason is because they have a long history and we can look way back many decades to see how these things have gone on. Now, I’m starting in 1966, which might seem like a strange starting point, but that when I was born. So there I am in 1966. How long does it take me to do this calculation. Now I have 100 billion neurons– that’s a lot of neurons in my brain– but unfortunately these neurons aren’t particularly good at doing mechanical floating-point operations.

1:21 - So I reckon I could issue operations at the rate of one Hertz. So that’s the frequency one Hertz, which is one operation per second, or one second per operation. Now remember, the core loop had three steps in it. So at one operation per second, or one second per operation, it’s going to take me three seconds to do that loop. And it turns out it would take me 650 years to add up the salaries of all 7 billion people in the world. And that’s really, for one person, it’s just not going to even complete in a lifetime. So it’s a completely untenable calculation.

1:54 - Now back in 1971, Intel introduced what is now considered to be the first modern integrated microprocessor, the Intel 4004, and it had 2000 transistors. Not many transistors compared to my 100 billion neurons, but these transistors are very good at doing floating-point calculations. The frequency of this machine with about 100 kilohertz– 100 kilohertz is 100,000 operations per second, or one operation every 10 microseconds– a microsecond is a millionth of a second. So every millionth of a second, this chip could do one operation. Now, remember, there are three steps, so it’s going to take 30 microseconds to do each loop. So, the total time to do that seven billion times is two and a half days.

2:35 - So even over four decades ago, microprocessors were able to translate what would have been a completely unfeasible calculation for one person to do into something which can be done in a few days. Which is a major step forward. But if we fast forward another two decades, we see the impact of Moore’s law. Moore’s law is this exponential increase, this regular doubling of the number of transistors you can get on a microprocessor, and in 1993 Intel released the Pentium, and in the intervening decades the manufacturing technology had increased to such an extent that they could now put 3 million transistors on the Pentium. This extra density of transistors translates into the ability to run these at a faster frequency.

3:15 - The Pentium had a frequency of 60 megahertz. That’s 60 million operations per second, or the time per operation is 17 nanoseconds, where a nanosecond is a billionth of a second. Now it’s worth thinking about that for a while. A nanosecond is an extremely short period of time. Let’s imagine a ray of light. Light is the fastest thing there can be in the universe and every nanosecond, light only travels about 30 centimetres. So, each time that the Pentium issues an operation, it issues an operation every 17 nanoseconds. In those 17 nanoseconds, light has only travelled about five metres, about the width of a room. Now it takes three steps per loop.

3:56 - So the time loop is 50 nanoseconds, which means, in 1993, it would have taken six minutes to add up the salaries of everybody in the whole world. So, you can see, in a couple of decades a calculation which would have taken several days, has gone to something which you could just set the computer going, and go off and have a cup of coffee and come back, and it would be finished. So, let’s fast forward to 2012, another two decades. And Intel then released the core i7 processor, which had three billion transistors– not three million but three billion. It could operate at a frequency of three gigahertz, which is three billion operations per second.

4:29 - The time per operation is a third of a nanosecond. So each time that the core i7 could issue an operation, every third of a nanosecond, light could only travel 10 centimetres. So, you can see we’re approaching some fairly fundamental physical limits here, in how fast processors can go. The time per loop was one nanosecond. That meant that to add up all seven billion salaries would have taken seven seconds. So, again, in the intervening two decades from 1993 to 2012, we’ve gone from a calculation where you would have to go away and wait to have a cup of coffee for it to finish, to something you could just sit and it would be ready almost instantaneously.

5:05 - So, hopefully that illustrates the impact of Moore’s law. How, from 1971 to 2012, over the period of four decades, this relentless increase in the speed of CPUs has gone from a calculation taking two and a half days, to taking seven seconds.
:::

So how long does it take to add up 7 billion numbers? Well, it depends on what you are using to add them…

We’ve talked about Moore’s Law and what it has meant in terms of processor hardware:

Every two years the number of transistors that can fit onto an integrated circuit doubles (until 2005);

Every two years the number of CPU-cores in a processor now doubles.

In this video David uses the income calculation example to illustrate what is the impact of the first point in practice.

---

![Podium with top three winners](images/winner-1019835_640.jpg)
*Image courtesy of [Peggy_Marco](https://pixabay.com/users/peggy_marco-1553824/) from [Pixabay](https://pixabay.com)*

## Top500 list: Supercomputing hit parade

On the top500.org site, you can find the ranks and details of the 500 most powerful supercomputers in the world.

The first Top500 list was published in June 1993, and since then the Top500 project publishes an updated list of the supercomputers twice a year:

in June at the ISC High Performance conference in Germany,
and in November at the Supercomputing conference in the US.
The site provides news with respect to supercomputers and HPC, it also has a handy statistics tool which people can use to gain more insights of the Top500 systems.

:::callout{variant="discussion"}
Have a look at the most recent list and briefly comment on the following question:

- What manufacturers produce the world’s largest supercomputers?
- What types of processors do they use?
- What fraction of peak performance is typically achieved for the LINPACK benchmark?
- Play with the statistics tool on top500.org and think about the trends in current HPC systems. For example, how many supercomputers in the Top500 are classed as being for use by industry?
:::

---

## Terminology recap

::::challenge{id=sc_world.1 title="Supercomputing World Q1"}
Historically, a ____ contained a single "brain" but nowadays it contains multiple ____.

:::solution

1) Processor

2) Cores

:::
::::

::::challenge{id=sc_world.2 title="Supercomputing World Q2"}
The mode of computing in which a single CPU-core is doing a single computation is called ____ computing, as opposed to ____ computing, where all CPU-cores work together at the same time.

:::solution

1) Serial

2) Parallel

:::
::::

::::challenge{id=sc_world.3 title="Supercomputing World Q3"}
The process of evaluating the performance of a supercomputer by running a standard program is called ____. The standard calculation used to compile the top500 list is called ____.

:::solution

1) Benchmarking

2) LINPACK

:::
::::
