---
name: Persistent Identifiers
dependsOn: ["software_project_management.fair.02_data_metadata_nondata"]
tags: []
learningOutcomes:
  - Understand what a persistent identifier is and how it works
  - Identify different types of persistent identifiers for different objects
attribution:
  - citation: Best Practices for Tombstone Pages
    url: https://support.datacite.org/docs/tombstone-pages
    image: https://files.readme.io/fb9347b-small-DataCite-Logo_secondary.png
    license: CC-BY-4.0
  - citation: What is a DOI?
    url: https://www.doi.org/the-identifier/what-is-a-doi/
    image: https://www.doi.org/images/logos/header_logo_cropped_registered.svg
    license: CC-BY-4.0
  - citation: ORCID
    url: https://orcid.org
    image: https://orcid.org/assets/vectors/orcid.logo.svg
    license: CC0 1.0
  - citation: Research Organization Registry (ROR)
    url: https://ror.org
    image: https://ror.org/img/ror-logo.svg
    license: CC-BY-4.0
---
## Introduction

Almost all of the FAIR guiding principles are not feasible without the
existence of persistent identifiers and this highlights their importance. In
this module, we are going to look at what they are and how they work. We will
also look at some common types of persistent identifiers that are used to
identify digital assets, researchers and research organisations.

## What is it?

An identifier is a unique reference to an object, regardless of where it is.
You can think of it as your passport number. Your passport number is unique to
you and it will remain the same wherever you are.

Nothing is inherently persistent or permanent, especially in the digital
world. In the case of an identifier, how persistent it is entirely depends on
the commitment from the people or organisations that create it.

With these in mind, we can define a persistent identifier as

> a long-lasting reference to a digital or non-digital asset that provides a
> globally unique name independent of its location.

## How does it work?

Behind all persistent identifiers, there are _resolution_ processes that
translate identifiers into the current location of the digital asset it refers
to. When you query a resolution system with a persistent identifier, it looks
it up in its registry, the registry returns the current location (e.g. URL)
pointing to the digital asset, and finally redirects you to the resource. For
non-digital assets, it will redirect you to the metadata, for example, a web
page that shows the name and current affiliation of the researcher.

When the digital asset is relocated, the information in the registry and all
the related metadata is updated. Human researchers or computational agents
always use the same identifier to query the same digital asset, no matter where
it is moved or how many times it has been moved. This solves the pain of
unresolvable URL of digital asset once it has been moved because of various
reasons and allows computational agents to discover the digital asset
autonomously, one of the foundational aspects of the FAIR principles.

If the underlying digital asset needs to be deleted, the convention is to
redirect users to a 'tombstone page' which displays all the metadata and
explains why it is removed. This is important for the digital asset to be
compliant with the 'To be Accessible' FAIR principle.

## Types of persistent identifiers

### Digital Object Identifiers (DOIs)

DOI is arguably the most familiar persistent identifier known to researchers as
it often appears together with journal articles. However, DOI is not limited to
publication only and it is also used for research datasets, software, protocols
and preprints etc.

A DOI always starts with '10' followed by a suffix. It was standardised as ISO
26324 and since its inception in 1997, it is estimated that DOIs have been
resolved over 123 billion times (at the time of this writing). Besides
standard DOIs, [shortdoi](https://shortdoi.org) creates shortened DOIs that are
ideal for modern research data sharing via social media and mobile platforms.

### Open Researcher and Contributor ID (ORCID)

ORCID is used to identify researchers with 16-digit identifiers in the format
of XXXX-XXXX-XXXX-XXXX. It solves the pain of researchers having to change
their names because of marriage or other legal reasons. It also prevents
confusion when a researcher has a common name (such as the surname Smith or
Wang, and given names such as Yi or Olivia), or different convention of the
same name (such as the order of surname and given name, whether to use
hyphenated form etc.).

ORCID is straightforward to register and as of 2022, there are over 15 million
registered records and the number continues to grow, showing wide adoption
across the research community.

### Research Organisation Registry (ROR)

ROR identifies research organisations with a 9-character string
(e.g. 052gg0110). Similarly to ORCID, it solves ambiguous naming of academic
institutions (e.g. University of Oxford or Oxford University) and renaming
because of historical or strategical reasons (e.g. 'UK Centre for Medical
Research and Innovation' was renamed to 'The Francis Crick Institute' in 2011).

ROR was launched in 2019 and in 2026 it contains more than 116 thousand
research organisations.

### Others

There are other kinds of persistent identifiers, such as [Archival Resource
Keys](https://arks.org) which is similar to DOIs but is cheaper and adopted a
more decentralised approach, and [RAiD](https://www.raid.org) which can be used
to identify a research project and serves as a pointer to other persistent
identifiers.

There are also software-specific persistent identifiers such as
[SoftWare Hash IDentifier (SWHID)](https://www.softwareheritage.org/software-hash-identifier-swhid/)
(standardised as ISO 18670 in 2025) and [bio.tools](https://bio.tools) specific
for software tools in bioinformatics and the life sciences.

## Conclusion

This module discusses what exactly persistent identifiers are and how they
work, followed by the major types and other kinds of persistent identifiers for
different use cases. Persistent identifiers are the foundation for the FAIR
principles. Without them, (meta)data cannot be discovered in a reliable way,
hence not findable. Persistent identifiers also enable access of (meta)data
through standardised communication protocol (HTTP) and provide an
inter-connected picture of your research outputs, such as linking the
researcher (by ORCID) to a dataset (by DOI) which was analysed by a certain
software (by DOI/SWHID). They also provide information for tracking its
provenance reliably and autonomously.
