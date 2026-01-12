---
name: Data, metadata and non-data digital assets
dependsOn: ["software_project_management.fair.01_understanding_fair"]
tags: []
learningOutcomes:
  - Understand 3 main types of digital assests: data, metadata and non-data digital assets
  - Explain what exactly are data, metadata and non-data digital assets
attribution:
  - citation: eCFR 200.315 Intangible property
    url: https://www.ecfr.gov/current/title-2/subtitle-A/chapter-II/part-200/subpart-D/subject-group-ECFR8feb98c2e3e5ad2/section-200.315
    image: https://huntersquery.byu.edu/wp-content/uploads/2021/03/Screen-Shot-2021-03-11-at-5.15.54-PM.png
  - citation: What do we mean by “data”? A proposed classification of data types in the arts and humanities
    url: https://doi.org/10.1108/JD-07-2022-0146
    image: https://emer.silverchair-cdn.com/data/SiteBuilderAssets/Live/Images/jd/Journal_of_Documentation-1722323768.svg
    license: CC-BY-4.0
  - citation: Horizon Europe Programme Guide
    url: https://ec.europa.eu/info/funding-tenders/opportunities/docs/2021-2027/horizon/guidance/programme-guide_horizon_en.pdf
    image: https://ec.europa.eu/info/funding-tenders/opportunities/portal/assets/ecl/ec/logo/positive/logo-ec--en.svg
    license: CC-BY-4.0
---
## Introduction

The FAIR principles are applicable to all research digital assests, and in this
module we are going to look at what constitute as digital assests and what we
can do to achieve FAIRness for them. For most of the scientific research,
digital assests can be roughly divided into 3 categories: data, metadata and
non-data digital assets.

## Data

If we define scientific research as the process of knowledge creation, it is
vital to document everything that are leading to it. With this in mind, we can
employ the following definitions of data:

> 'recorded factual material ... as necessary to validate research' (200.315
> Intangible property, Code of Federal Regulations, US Federal Government)

> everything that are 'linked to the workflow of knowledge creation' (Gualandi
> B, Pareschi L, Peroni S (2023))

What count as data varies a lot across different scientific domains and often
depends on context. For instances, a field geologist may treat a rock itself as
data while a geochemist only considers the chemical composition of the rock as
data. It is thus crucial for researchers to identify precisely what entities
should be considered as data in their project.

### Data Management Plan

A lot of funding agencies require a submission of a document called data
management plan (DMP) before the grant application process or during the
research project. We will employ this definition for DMP:

> '... formal documents that outline from the start of the project all aspects
> of the research data lifecycle, which includes its organisation and curation,
> and adequate provisions for its access, preservation, sharing, and eventual
> deletion, both during and after a project. (Horizon Europe Programme Guide,
> p. 48)

As a DMP considers aspects such as what data should be collected or generated
in different phases of the research project, even if a DMP is not mandatory in
the grant application process, you are strongly encouraged to create one as
having even an informal plan about research data management will help you to
identify potential problems earlier.

There are tools that help the creation of DMPs:

- [DMPonline](https://dmponline.dcc.ac.uk) (Digital Curation Centre, UK)
- [DMP Tool](https://dmptool.org) (California Digital Library, US)

This module will not go into detail about the creation of DMPs but you can
browse some of the public plans available (
[from DMPonline](https://dmponline.dcc.ac.uk/public_plans)
or [from DMP Tool](https://dmptool.org/public_plans)) to get an idea about what
information a DMP should be included. Some core questions that you should
consider are:

- what data will be collected/generated?
- what is the data collection/generation methodology?
- what are their formats?
- how do you organise the data (e.g. file naming, storage)?
- how will you share your data?
- is there any restriction about the sharing of data?
- how to measure the quality of the data?
- how will you preserve the data?
- for how long the data should be preserved?
- who will be responsible?
- what metadata will be included?

Thinking through the above questions will help you to anticipate potential
issues, clarify the whole research project and plan ahead so you can adhere to
the FAIR principles in a systematic way.

## Metadata

Metadata is data about data and it forms a core part in the FAIR principles as
it is metadata that can make data machine-actionable. Without sufficient and
accurate metadata, computational agents cannot understand the data
characteristics correctly and make use of them automatically.

The line between data and metadata is not clear and again depends on context,
for instance, the energy used to acquire an image in electron microscopy is
metadata if the research focuses on certain features present on the image;
however, if the research is about how the signal-to-noise ratio varies with the
energy used, then the energy itself becomes the data.

### Dublin Core

Dublin Core is a simple and universal collection of vocabularies for describing
digital assets, i.e. metadata. The wide adaptation of Dublin Core is important
for 'To be Interoperable' in the FAIR principles.

There are 15 core elements in Dublin Core and they are very generic to allow
description of data in a wide range of scientific domain. it is usually a good
starting point to include them as metadata:

- title
- subject
- description
- type
- source
- relation
- coverage
- creator
- publisher
- contributor
- rights
- date
- format
- identifier
- language

You are recommended to look at [the specification](https://www.dublincore.org/specifications/dublin-core/dces/)
for the above fields. All of the above fields are optional and repeatable, e.g.
'language' is not applicable if the data is an image; 'contributor' can be
repeated if there are more than one.

### Choosing a Metadata Standard

While Dublin Core is one of the most popular metadata standard, we often
require some extensions for our research project. Across different domains,
there are a lot of metadata standards in the community and it is important to
select one that is appropriate to your scientific domain and the research
project. Below are some places that could help you to explore metadata
standards in your domain:

- [Digital Curation Centre](https://www.dcc.ac.uk/guidance/standards/metadata)
- [RDA Metadata Standards Catalog](https://rdamsc.bath.ac.uk)
- [FAIRsharing](https://fairsharing.org/search?fairsharingRegistry=Standard)

## Non-data Digital Assets

Modern scientific research often generate outputs that are not traditionally
considered as data but essential in the FAIR principles and for other
researchers to validate the results, so it is important to recognise those
research outputs and make them FAIR.

Below is a list what non-data digital assets could mean:

- software
- computational workflows
- algorithms
- protocols
- electronic lab notebooks
- computational environment/containers
- AI/ML models
- research plans
- API docs
- preprints

Extensions to the FAIR principles may exist for some of the above research
outputs, for instance, [FAIR4RS](https://doi.org/10.1038/s41597-022-01710-x) is
an extension to the FAIR principles for research software. Websites such as
[protocols.io](https://www.protocols.io) supports sharing of methods and
workflows, and registries like [Docker Hub](https://hub.docker.com) contains a
lot of computational environment that allow researchers to reproduce analysis
in a convenient way.

## Conclusion

There are many kinds of research digital assets and we can roughly divide them
into 3 categories: data, metadata and non-data digital assets. This module
explains what they are, how you can make them more FAIR and resources that are
useful for further exploration. By identifying all the research outputs from
your project, you can apply the FAIR principles to them early on in your
project.
