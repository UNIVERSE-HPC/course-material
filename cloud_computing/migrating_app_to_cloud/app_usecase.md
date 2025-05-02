---
name: Application Use Case Overview
dependsOn: []
tags: []
learningOutcomes:
---

For this course, we will use a running application as a practical example to demonstrate key concepts. The **Image Processing Application** will serve as our use case, showcasing how to manage file uploads, image processing, and metadata storage. While image processing applications can often be complex, this is a streamlined, lightweight version specifically designed and developed to demonstrate essential functionality without adding the complexity of a full-scale image processing system.

## Image Processing Application

The running application (i.e., the image processing application) is built using [Flask](https://flask.palletsprojects.com/en/stable/), a lightweight web framework that simplifies the development of web applications by providing essential tools for routing, request handling, and templating. The application allows users to upload images, after which the image processing component kicks in to perform necessary transformations on the images. Both the original and processed versions are then stored. Additionally, the application saves metadata—such as filenames, sizes, and upload dates—in a [SQLite](https://www.sqlite.org/) database, showcasing file handling and database interaction.

### Application Features and Workflow

We will walk through the main features and the workflow of the application, from image upload to metadata storage and display.

1. **Upload and Processing**: Users upload images through the web interface. Uploaded images are saved in the `uploads` directory, and metadata is stored in the SQLite database.
2. **Image Processing**: Images are converted to grayscale and resized to 300x300 pixels. The processed images are saved in the `processed` directory.
3. **Metadata Storage**: Details of both the original and processed images are stored in the SQLite database.
4. **Display and Download**: A dashboard displays all image metadata, and users can download processed images from the web interface.
