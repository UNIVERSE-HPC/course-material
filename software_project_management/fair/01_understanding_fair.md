## Introduction

This module aims to give you an overview of the FAIR principles for scientific
data. We will start with some scenarios that researchers frequently face, the
motivation of the FAIR principles and their implication to your research. By
the end of this module, you will be able to explain the FAIR principles in your
own words and identify any existing FAIRness gaps in your data with some
recommendations to improve it.

## Common Scenarios

As a researcher, the following scenarios may be familiar to you:

- You want to replicate some analyses by other researchers but either the data
  is lost or incomplete.
- You are not sure if you can reuse or how to access those dataset even they
  are available.
- There is no documentation or metadata associated with the dataset which you
  are going to reuse.
- You do not know how other researchers obtained or generated the data
  (provenance)
- You try to explore dataset similar to your research but you have no clue
  about how to locate relevant dataset.
- You have your paper published but not sure where or how to put the associated
  data for others to verify or reuse.

The FAIR principles data aim to alleviate some of the above pains arising from
the above scenarios.

## FAIR Guiding Principles

FAIR is composed of 4 foundational principles:

1. To be Findable
2. To be Accessible
3. To be Interoperable
4. To be Reusable

Modern scientific research often involve handling data that far exceed the
processing capacity of humans in terms of their volume, throughput and
complexity. As a result, these principles emphasise on machine-actionability by
requiring explicit structures, i.e. not only humans can find and interpret the
(meta)data but also enable computational agents to autonomously discover and
act upon them to ensure vast amount of data can be processed continuously and
reproducibly.

### To be Findable

This is to make your data discoverable by both humans and machines.

> - (meta)data are assigned a globally unique and persistent identifier
> - data are described with rich metadata
> - metadata clearly and explicitly include the identifier of the data it
>   describes
> - (meta)data are registered or indexed in a searchable resource

#### What Findable means

It is important to assign the data with a *globally unique* and *persistent*
identifier: 'globally unique' means this identifier is associated with this
data only; 'persistent' means it remains valid indefinitely. This makes it
resolvable and computational agents can use this identifier to automatically
retrieve the data. Persistent identifiers are one of the core parts of FAIR and
will be discussed in an upcoming module.

There is no universal standard that defines when metadata have attained a
certain level of richness as how much is *rich* varies by scientific domains.
The consensus is that the metadata should provide enough information for both
humans and critically, computational agents, to determine the data's usage
restriction, quality, collecting and analysis methodology, provenance, and
other information specific to a domain (ideally defined by a standard).

To promote your data, it is important to make your (meta)data discoverable by
'registered or indexed in a searchable resource'. This means (meta)data should
be deposited in systems that enable discovery, which could be a general
repository, a domain-specific database, an institutional server or a registry.
They should be searchable by both humans and computational agents, which
usually accompany with programmatic access and protocols. We will look at this
in detail in an upcoming module.

#### Be more Findable

- Assign persistent identifiers to all of your published (meta)data.
- Deposit data in a reputable repository rather than personal website or
  server.
- Use metadata templates from data repository or domain-specific standard to
  maximise the information you could provide.

### To be Accessible

This is to provide clear instructions on how to access the data, even the
access is restricted.

> - (meta)data are retrievable by their identifier using a standardized
>   communications protocol
>     - the protocol is open, free, and universally implementable
>     - the protocol allows for an authentication and authorization procedure,
>       where necessary
> - metadata are accessible, even when the data are no longer available

#### What Accessible means

It focuses on how humans or computational agents can access the (meta)data.
With persistent identifiers, a communication protocol should be available to
enable (meta)data access. The *protocol* should have *freely available open*
specification, widespread adoption and should be well documented that anyone
with the necessary technical knowledge can implement it.

:::callout{variant="note"}
As a researchers, we seldom need to implement the authentication/authorisation
protocol itself but it is important to select one that adheres to the
'Accessible' principle.
:::

Necessary but minimal authentication/authorisation procedures can be included
for data such as proprietary or private information. Those procedures should
allow computational agents to act on it automatically: extract the requirement
of accessing the data from the metadata, and authorise the human researchers if
valid credentials are present, or alert the human researchers if a more manual
authentication/authorisation process is required (such as approval from a
committee/company).

:::callout{variant="note"}
Data that are not 'freely available' or 'open' can be 'accessible' if the
procedures to retrieve them are clearly documented and understood by both
humans and computational agents, so sensitive or private data can still be
accessible by the definition in FAIR.
:::

It is inevitable that some scientific data will no longer be available over
time owing to ongoing storage and maintenance cost of infrastructure or
permission changes mandated by legal/commercial requirement. Metadata, on the
other hand, are generally much easier to store and maintain as they are smaller
in size and do not contain sensitive information as the data themselves. The
access to metadata should still be maintained even the actual data is gone as
metadata are still valuable for future researchers to track down what research
have been done and their methodologies, which are particularly important for
literature review and provenance purposes.

#### Be more Accessible

What you can do can vary a lot with the nature of your data and below are
some recommendations that you can explore to make your data more 'accessible':

##### Open Data

- Make (meta)data retrievable via HTTP/HTTPS.
- Use repositories that implement public API for computational agent to query
  and download (meta)data.
- Use reputable repositories that guarantee long-term availability.

##### Controlled-Access Data

- Include public metadata about what data exist, who collected them, and how to
  request access etc.
- For sensitive data, include the access requirement such as fees, licence,
  institutional/committe/company approval, and information about restricted
  usage etc.
- For embargoed data, include the embargo period and use repositories that
  automatically update its permission.

### To be Interoperable

This is to enable data to work with other data.

> - (meta)data use a formal, accessible, shared, and broadly applicable
>   language for knowledge representation.
> - (meta)data use vocabularies that follow FAIR principles
> - (meta)data include qualified references to other (meta)data

#### What Interoperable means

A computational agent does not naturally possess semantic understanding on
(meta)data as humans do, so it is not able to infer the underlying meaning of
the (meta)data to decide what appropriate analyses or actions to perform. If
(meta)data from different sources can work together automatically, we can
maximise the benefits provided by computational agents.

It is important to use *formal* languages which have clearly defined structures
and interpretable rules so a computational agent can leverage them for
autonomous data processing, in contrary to free-text description.
'*Accessible*' in this context ensures the above specification of languages is
publicly available and easily retrievable. The specification should also be
widely adopted by the scientific community (*shared*) and be general (*broadly
applicable*), in contrary to a specification that is only used by a small
number of research groups in a niche area.

The vocabularies in the language specification should themselves be FAIR:
persistent identifiers should associate with individual terms and contain rich
metadata to describe them; the vocabularies should be accessible via a
standardised communication protocol; the relationship among vocabularies
should be clearly documented.

In modern scientific research, (meta)data seldom exist in isolation and it is
important to establish meaningful connections (*qualified references*) among
them. A standardised way is needed to describe the relationship such as
'dataset A is a subset of dataset B or 'result C from experiment D validates
hypothesis E'. The qualified references can be used by a computational agent to
enable automatic discovery of related resources and trace back to the original
source, which is useful for literature reviews and provenance purposes.

#### Be more Interoperable

- Use standard file formats such as JSON, CSV or specialised formats widely
  used in a particular scientific domain to facilitate data exchange.
- Replace free-text terms by controlled vocabularies for (meta)data.
- Document relationship extensively among data using appropriate language
  specification.

### To be Reusable

This is to provide comprehensive instructions on how to properly reuse the
data.

> - meta(data) are richly described with a plurality of accurate and relevant
>   attributes
>     - (meta)data are released with a clear and accessible data usage license
>     - (meta)data are associated with detailed provenance
>     - (meta)data meet domain-relevant community standards

#### What Reusable means

All the previous principles lay the foundation for data reuse by both humans
and computational agents. If future researchers want to reuse the data, it is
helpful for them to be able to retrieve useful information and guidance from
the (meta)data. It is important to provide as much information as possible so
it can be used in a different context with minimal effort.

A licence should be attached to the (meta)data so it is obvious for both humans
and computational agents to determine whether they can use the data and any
restriction associated with it. Provenance should be extensively documented to
provide information about the data sources and methodologies that generate the
data, so it is possible to assess the quality and fitness-for-purpose when the
data is expected to be applied in a different context.

A specific domain may already have adopted certain frameworks or agree with
some conventions, so it is important to follow established domain-specific
practices so those familiar with the domain can quickly understand and assess
your data for its suitability.

#### Be more Reusable

- Be generous in providing information related to the data as something look
  unnecessary to you may be paramount to others, for examples:
    - descriptive metadata such as what data was collected, its rationale and
      any access restriction,
    - structural metadata such as the relationship among the data,
    - technical metadata such as the file format (e.g. the version of a
      standardised file format)
- Choose a well-established licence with a standardised identifier.
- Include basic provenance information such as the researchers (e.g. by
  persistent identifiers) and funding sources (e.g. by grant numbers).
- Use a widely-adopted standard in your domain.

## Conclusion

We have looked at the 4 foundational principles in FAIR (Findable, Accessible,
Interoperable and Reusable) and elaborated on their underlying meaning. Some
actionable items were included for each of the principles as well. The
following modules aim to look into some specific area mentioned in this
overview.

The FAIR Guiding Principles are intentionally domain-independent and very high
level, and they do not have any preferences in implementation (such as
technology, software or standard). The principles themselves are not a standard
or specification, they are guidelines that help researchers to better promote
their digital research artefacts. The FAIRness of your data exists in a
continuous spectrum rather than as a binary property and you are encouraged to
incrementally make your data more FAIR.
