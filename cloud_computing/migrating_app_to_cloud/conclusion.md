---
name: Conclusion
dependsOn: []
tags: []
learningOutcomes:
---

This course was designed to introduce you to cloud computing, with a focus on leveraging AWS services to enhance an application's scalability, performance, and manageability. Adopting a hands-on approach, we began with a locally hosted application and progressed through to a full cloud deployment, allowing you to understand each stage of the cloud integration journey.

The course was structured into three key parts:

1. **Local Application Setup**: We started with an [Application Use Case Overview](app_usecase.md) and explored [Running the Image Processing Application](app_local_storage.md) in an on-premises environment. The applicaiton was developed using **Flask** for the web interface and **SQLite** for local metadata storage. This section covered core concepts such as file uploads, image transformation, and metadata management. It served as a foundation for understanding how traditional applications operate in a local environment.

2. **Integrating AWS Services with a Local Application**: In this part, we kept the application running locally but integrated it with **AWS Services** for remote data storage and management (aka [Integrating AWS Services with the Image Processing Application](app_remote_storage.md)). We introduced **S3** for scalable image storage and **DynamoDB** for managing metadata in the cloud, allowing the local application to leverage these cloud resources. This setup demonstrated how AWS services could enhance durability and accessibility even for applications not fully deployed on the cloud. We also explored the [Drawbacks of Running the Image Processing Application with Remote AWS Services](app_onprem_limitation.md), noting that full migration to AWS could optimize performance by reducing latency and streamlining management.

3. **Migrating the Application to AWS with Elastic Beanstalk**: In the final part, we completed the [Application Migration to AWS with Elastic Beanstalk](app_migrate_to_aws.md), creating a fully managed cloud-native environment. This migration shifted responsibilities for scaling, resource allocation, and management to Elastic Beanstalk, providing a production-ready deployment with minimal overhead and demonstrating the advantages of running the application directly alongside its data in the cloud.

Through this course, you have developed both practical skills and foundational knowledge in building, integrating, and deploying applications using AWS. While we focused on three core services in this course, AWS offers a wide array of services across compute, storage, databases, machine learning, and more. Each cloud provider, including AWS, Azure, and GCP, provides an evolving ecosystem of tools to meet diverse application and infrastructure needs.

This course represents just the beginning. With the foundational concepts and hands-on experience you have gained, you are now well-prepared to explore more advanced services and architectures, and to design scalable, resilient, and cloud-native applications for a variety of use cases.