---
name: Conclusion
dependsOn: []
tags: []
learningOutcomes:
---

The goal of this course was to familiarize you with cloud computing, particularly with using AWS services to enhance an application’s scalability and performance. We took a hands-on approach, beginning with a **local setup** and gradually transitioning to a full cloud deployment.

The course was structured into three main parts:

1. **Local Application Setup**: We started with an [Application Use Case Overview](app_local_storage.md) of a local image processing application built with **Flask** to handle the web interface and **SQLite** to store metadata locally. This setup introduced you to fundamental processes like file handling, image processing, and metadata management in a Python-based application.

2. **Integrating AWS Services with a Local Application**: In this part, we kept the application running locally but integrated it with **AWS Services** for remote data storage and management (aka [Running a Local Application with Remote AWS Services](app_remote_storage.md)). We introduced **S3** for scalable image storage and **DynamoDB** for managing metadata in the cloud, allowing the local application to leverage these cloud resources. This setup demonstrated how AWS services could enhance durability and accessibility even for applications not fully deployed on the cloud. We also explored the [Drawbacks of Running a Local Application with Remote AWS Services](app_onprem_limitation.md), noting that full migration to AWS could optimize performance by reducing latency and streamlining management.

3. **Migrating the Application to AWS with Elastic Beanstalk**: In the final part, we [migrated the application fully to AWS using Elastic Beanstalk](app_migrate_to_aws.md), creating a cloud-native, fully managed environment. This migration shifted responsibilities for scaling, resource allocation, and management to Elastic Beanstalk, providing a production-ready deployment with minimal overhead and demonstrating the advantages of running the application directly alongside its data in the cloud.

Through this course, you’ve gained practical skills and theoretical knowledge in building, integrating, and deploying applications on AWS. These foundational skills provide a strong base for further exploration in developing scalable, high-performing applications on the cloud.
