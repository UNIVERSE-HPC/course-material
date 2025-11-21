---
name: Parallel Computing
id: parallel_computing
dependsOn: [high_performance_computing.parallel_computers]
files: [01_intro.md, 02_programming.md, 03_parallel_performance.md, 04_practical.md]
summary: |
  This module covers how supercomputers are programmed to make use of computational resources in parallel to perform
  calculations more quickly.
---

In this video David will give a brief description of what awaits you in this module about parallel computing.

::::iframe{id="kaltura_player" width="700" height="400" src="https://cdnapisec.kaltura.com/p/2010292/sp/201029200/embedIframeJs/uiconf_id/32599141/partner_id/2010292?iframeembed=true&playerId=kaltura_player&entry_id=1_j7i8ueqz&flashvars[streamerType]=auto&amp;flashvars[localizationCode]=en&amp;flashvars[leadWithHTML5]=true&amp;flashvars[sideBarContainer.plugin]=true&amp;flashvars[sideBarContainer.position]=left&amp;flashvars[sideBarContainer.clickToClose]=true&amp;flashvars[chapters.plugin]=true&amp;flashvars[chapters.layout]=vertical&amp;flashvars[chapters.thumbnailRotator]=false&amp;flashvars[streamSelector.plugin]=true&amp;flashvars[EmbedPlayer.SpinnerTarget]=videoHolder&amp;flashvars[dualScreen.plugin]=true&amp;flashvars[Kaltura.addCrossoriginToIframe]=true&amp;&wid=1_54nq03w8" allowfullscreen webkitallowfullscreen mozAllowFullScreen allow="autoplay _; fullscreen _; encrypted-media \*" sandbox="allow-downloads allow-forms allow-same-origin allow-scripts allow-top-navigation allow-pointer-lock allow-popups allow-modals allow-orientation-lock allow-popups-to-escape-sandbox allow-presentation allow-top-navigation-by-user-activation" frameborder="0" title="Welcome_to_Parallel_Computing"}
::::

:::solution{title="Transcript"}
0:11 - We’ve mainly talked about hardware in the first two weeks. Now we’re going to focus on software. We’ll take a couple of simple examples and consider how you could split up each calculation to take advantage of a shared or distributed memory parallel computer. We’ll cover some of the key issues of parallel computing this week at a conceptual level. Again, the analogy of different people working on whiteboards is very useful to illustrate the core concepts. One of the examples we’ll use which is a very simple way of simulating the way the traffic flows on a road is something I hope you’ll find interesting to illustrate how we can use computer simulation to make predictions about the real world.

0:48 - Now the way the traffic simulation is parallelised will allow us to look at what overheads are introduced by parallelisation and enable us to start to quantify when running on a parallel computer is worthwhile and when it isn’t.
:::

We have previously discussed supercomputers mostly from a hardware perspective,
in this part we will focus more on the software side i.e. how supercomputers are programmed.

We begin by using the traffic simulation to illustrate the core concepts of parallel computing.
From there, we delve into different programming models and their relationship to machine architectures and how they can be applied to our case study.
Finally, we examine performance, learning how to evaluate whether our simulations are efficiently utilizes computing resources.
