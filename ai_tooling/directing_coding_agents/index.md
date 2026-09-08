---
name: Directing coding agents
id: directing_coding_agents
dependsOn: [
   introductory_courses.python,
   technology_and_tooling.version_control,
   technology_and_tooling.testing,
   technology_and_tooling.packaging_dependency_management,
   ]
files: [
   01_prompt_design.md,
   02_specifying_a_problem.md,
   03_specifying_a_feature.md,
   04_inherit_research_codebase.md,
   ]
learningOutcomes:
  - Design prompts that give a coding agent the context, boundaries and success conditions it needs
  - Decide whether a bug report or feature request is specified well enough for a coding agent to act on
  - Use a coding agent as an assistant to handle an inherited research codebase
summary: |
  Modern coding agents will fix, implement and explain anything you ask, whether or not you ask for the right thing. Using issues, feature requests and codebases from real-world scientific software, this course teaches you how to direct coding agents effectively to leverage their power.
---

Before attempting the material in this course, you should be familiar with the
fundamentals of software engineering such as version control, the idea of
software testing, and package management etc. If not, complete those
prerequisites first as you will not be able to fully appreciate and utilise the
power of coding agents (this course is not about vibe coding). A working
knowledge of Python is also required, at least you need to understand basic
structure of a Python script and know how to read a traceback. There are four
modules in this course and they should be gone through in this order:

1. [Prompt design](/ai_tooling/directing_coding_agents/01_prompt_design):
   learn how to design a prompt to guide a coding agent.
2. [Specifying a
   problem](/ai_tooling/directing_coding_agents/02_specifying_a_problem): rate
   bug reports and learn how to direct a coding agent to address a bug.
3. [Specifying a
   feature](/ai_tooling/directing_coding_agents/03_specifying_a_feature):
   implement a feature with a coding agent.
4. [Handling legacy research
   codebases](/ai_tooling/directing_coding_agents/04_inherit_research_codebase):
   use a coding agent to onboard a legacy research codebase you have inherited.

## Tools you will need

- Modules 1 to 3: a text editor.
- Module 4: any coding agent you can run on a local clone of a codebase. To
  prevent hitting the session limit if you have a subscription plan, use the
  most lightweight model it provides for the sake of this training, e.g. the
  Luna family from OpenAI and the Haiku family from Anthropic.
