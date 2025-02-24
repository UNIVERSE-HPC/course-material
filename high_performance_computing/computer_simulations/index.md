---
name: Computer Simulations
id: computer_simulations
dependsOn: [
    high_performance_computing.parallel_computing,
]
files: [
    00_practical.md,
    01_intro.md,
    02_weather_simulations.md,
    03_towards_future.md,
]
summary: |
    This module introduces computer simulations, using a number of examples, which are used to explore the behaviour
    of a real-world system represented as a mathematical model.

---

In this video David will give a brief description of what awaits you in this module about computer simulations.

# Welcome to Part 4

::::iframe{id="kaltura_player" width="700" height="400" src="https://cdnapisec.kaltura.com/p/2010292/sp/201029200/embedIframeJs/uiconf_id/32599141/partner_id/2010292?iframeembed=true&playerId=kaltura_player&entry_id=1_uo9lyoxr&flashvars[streamerType]=auto&amp;flashvars[localizationCode]=en&amp;flashvars[leadWithHTML5]=true&amp;flashvars[sideBarContainer.plugin]=true&amp;flashvars[sideBarContainer.position]=left&amp;flashvars[sideBarContainer.clickToClose]=true&amp;flashvars[chapters.plugin]=true&amp;flashvars[chapters.layout]=vertical&amp;flashvars[chapters.thumbnailRotator]=false&amp;flashvars[streamSelector.plugin]=true&amp;flashvars[EmbedPlayer.SpinnerTarget]=videoHolder&amp;flashvars[dualScreen.plugin]=true&amp;flashvars[Kaltura.addCrossoriginToIframe]=true&amp;&wid=1_h70phwce" allowfullscreen webkitallowfullscreen mozAllowFullScreen allow="autoplay *; fullscreen *; encrypted-media *" sandbox="allow-downloads allow-forms allow-same-origin allow-scripts allow-top-navigation allow-pointer-lock allow-popups allow-modals allow-orientation-lock allow-popups-to-escape-sandbox allow-presentation allow-top-navigation-by-user-activation" frameborder="0" title="Welcome_to_Computer_Simulations"}
::::

:::solution{title="Transcript"}
0:12 - From the first three weeks, you should now have a good understanding of what supercomputers are, how they’re built, and how they’re programmed. However, other than the traffic simulation, we haven’t covered how computers are used to simulate the real world. This week we’ll cover the basic concepts of computer simulation– the methods, the approximations, and the end-to-end process from inputting the initial data to visualising the final results. We’re going to use weather forecasting as a key example. Now it’s a field that’s always been at the forefront of computer simulation, and it’s an area where supercomputers are absolutely central in letting you know in advance whether you should have an outdoor barbecue tomorrow, or order pizza and eat inside.

0:54 - We won’t go into the details of parallelisation, but from what you’ve learned so far, you should be able to start to think about how these simulations can be broken down into many separate tasks, and then mapped onto a large parallel supercomputer.
:::

In the previous sections we talked about the hardware of supercomputers and how to program them, in this part we will focus on computer simulations. We will use weather simulations to illustrate the key concepts.
