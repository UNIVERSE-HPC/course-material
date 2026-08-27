---
name: Assessing FAIR
dependsOn: ["software_project_management.fair.01_understanding_fair","software_project_management.fair.02_data_metadata_nondata","software_project_management.fair.03_persistent_identifiers","software_project_management.fair.04_data_repository"]
tags: []
learningOutcomes:
  - Recognise there are different tools to assess the FAIRness of a research project and digital asset
---
## Assessing FAIR

We have introduced the 4 foundational FAIR guiding principles in [Understanding
FAIR](01_understanding_fair.md), what can be treated as a digital asset:
various kinds of research data, different standards of metadata and what could
be non-data digital objects in [Data, metadata and non-data digital
assets](02_data_metadata_nondata.md). Two types of infrastructure that are
essential to the FAIR principles: [Persistent
Identifiers](03_persistent_identifiers.md) and [Data
Repositories](04_data_repository.md) were also discussed.

After having a basic understanding of the FAIR principles and some of the
related concepts, you can evaluate the 'FAIRness', how well a particular
research project or digital asset adheres to the 4 FAIR principles. There are
different tools that you can choose to assess FAIRness, some are manual while
others are automated. However, it is important to recognise that achieving
FAIRness is often an incremental process and should not be treated as a
one-time goal.

### Manual tools

- [FAIR-Aware](https://fairaware.dans.knaw.nl) is a simple questionnaire
  developed by DANS (Data Archiving and Networked Services) in the Netherlands
  that tests your understanding of FAIR principles so you can make your project
  more FAIR.
- Similarly, [FAIR Data Self-Assessment
  Tool](https://ardc.edu.au/resource/fair-data-self-assessment-tool/) developed
  by ARDC (Australian Research Data Commons) contains 12 questions to help you
  self-assess the FAIRness of your project.
- The [Jisc report on FAIR](https://zenodo.org/records/1245568) contains a FAIR
  checklist in Appendix C which can be used to self-assess your research
  project. Appendix D contains a list of questions that you can ask yourself to
  get an idea of how FAIR your project is.
- If software is involved in your research project, you will find [the five
  recommendations from the Netherlands eScience Center and
  DANS](https://fair-software.nl) useful. This can be used as a checklist to
  assess the FAIRness of the software you use in your research.
- [ACME-FAIR: a guide for Research Performing
  Organisations](https://catalogue.fair-impact.eu/resources/acme-fair) is a
  comprehensive guide that contains 7 parts leading to FAIRness. Each part
  contains a checklist with checkboxes for you to indicate the current status
  (current/considering) regarding the FAIRness of your project.

### Automated tools

- [F-UJI](https://www.f-uji.net) accepts links to digital assets in the form of
  persistent identifier or URL and provides a summary of its FAIRness. There
  are 4 FAIR levels from this tool: _incomplete_, _initial_, _intermediate_ and
  _advanced_, which provide a quick overview of the FAIRness of the digital
  asset.
- [FAIR-Checker](https://fair-checker.france-bioinformatique.fr) from the
  French Institute for Bioinformatics provides a score (0 to 100) for each of
  the 4 FAIR principles. It can also inspect the metadata of the digital asset
  and compare them with established standards and ontologies.
- [FAIR Evaluation
  Services](https://fairsharing.github.io/FAIR-Evaluator-FrontEnd/#!/#%2F!)
  provides tests against [Maturity
  Indicators](https://doi.org/10.1038/sdata.2018.118) for data and metadata.
  Depending on your needs, you can choose different Maturity Indicators during
  the evaluation.

## Conclusion

Whenever possible, one should progressively adopt measures that make your
existing research project and digital asset more FAIR, such as depositing
relevant research objects into data repositories or enriching the metadata you
recorded. The degree of FAIRness from the above tools should be treated as
indicative only and it matters more for you to reflect on what is missing
during the assessment so you can gradually improve.

This concludes the 5 modules about the FAIR guiding principles for scientific
data. Institutions usually provide additional services such as consultancy or
training about data management through the library or dedicated departments and
you are always encouraged to explore more. These modules aim to convince you
that by following the FAIR principles, you can increase the credibility of your
research by treating digital assets as first-class research objects. Happy
FAIRing!
