---
name: Data Repositories
dependsOn: ["software_project_management.fair.03_persistent_identifiers"]
tags: []
learningOutcomes:
  - Understand the importance of data repositories in FAIR
  - Make an informed decision when choosing a data repository for a research project
attribution:
  - citation: Registry of Research Data Repositories
    url: https://www.re3data.org
    image: https://www.re3data.org/images/img/re3datalogo_black.png
    license: CC-BY-4.0
  - citation: FAIRsharing
    url: https://fairsharing.org
    image: https://fairsharing.org/assets/fairsharing-logo.svg
    license: CC-BY-4.0
---
## Introduction

Data repositories are platforms for publishing and long-term preservation of
scientific data and are central to the FAIR guiding principles. When you
deposit your data in a data repository, not only your data are stored reliably,
but also you get automatically assigned a persistent identifier to each data
record and this can be cited in a publication, which is essential for the data
to be findable. Depending on the data repository, you can populate the metadata
fields by using different kinds of standards, which is important for making the
data accessible, interoperable and reusable.

## Choosing a data repository

Data repositories can be roughly divided into two categories: general and
domain-specific. When choosing which data repository to use, the first step is
to check if there is any requirement in your research project to use a specific
one. If not, it is often preferable to use a domain-specific repository rather
than a general one if that exists because it can maximise the visibility in
your community. The choice of data repository should be made as early as
possible, ideally during the completion of the [data management
plan](02_data_metadata_nondata.md#Data Management Plan), so during your
research you can ensure sufficient metadata are collected and documented.

When choosing a data repository, we need to ensure it is a trustworthy one. The
[CoreTrustSeal Trustworthy Data Repositories
Requirements](https://www.coretrustseal.org) contains 16 requirements that a
reliable data repository would demonstrate. As of 2026, there are over 160 data
repositories have been certified and when there are alternatives, certified
data repositories are often preferred.

::::challenge{id=data-repo-q1 title="Data repo Q1"}
Go to [re3data](https://www.re3data.org) and
[FAIRsharing](https://fairsharing.org/search?fairsharingRegistry=Database) to
explore data repositories that are specific to your domain. Consider also
looking at some of the key papers in your field and check where they deposit
their research data associated with the publication.
::::

## Popular data repositories

You can deposit your research data in a general data repository when there is
no obvious choice in your domain. The [Generalist Repository Ecosystem
Initiative (GREI)](https://datascience.nih.gov/data-ecosystem/generalist-repository-ecosystem-initiative)
consists of 7 established general data repositories which share common metadata
schemas and standardised metrics with different tooling around them. Although
they are all safe and popular choices, general data repositories are not
limited to these 7 in GREI and you should choose one that is relevant to your
research. Below the 7 data repositories in GREI are listed:

- [Dataverse](https://dataverse.harvard.edu)
- [Dryad](https://datadryad.org)
- [Figshare](https://figshare.com)
- [Mendeley Data](https://data.mendeley.com)
- [Open Science Framework](https://osf.io)
- [Vivli](https://vivli.org)
- [Zenodo](https://zenodo.org)

Different repositories have different features and limitations, for instance,
each record in [Zenodo](https://zenodo.org) is limited to 50GB;
[Figshare](https://figshare.com) has an overall limit of 20GB for individual
users while institutional users could have up to 5TB;
[Dataverse](https://dataverse.harvard.edu) is free up to 1TB with each record
limited to 2.5GB. While [Vivli](https://vivli.org) is a member of GREI, it is
used primarily for preserving clinical research data.

::::challenge{id=data-repo-q2 title="Data repo Q2"}
Look at the document [Best Practices for Data Submission in Generalist
Repositories: A Checklist](https://zenodo.org/records/14278907) for some
guidelines when depositing data in a general repository.
::::

## Conclusion

This module starts with discussing what a data repository is and the difference
between ordinary storage: long-term preservation and assignment of persistent
identifiers, which enables the FAIR guiding principles. It is followed by some
guidance about how to choose a data repository and provides some popular data
repositories as examples.
