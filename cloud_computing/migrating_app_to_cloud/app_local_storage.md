---
name: Running the Image Processing Application
dependsOn: []
tags: []
learningOutcomes:
---

This image processing application is designed to store and process images using local storage, with a SQLite database specifically for metadata management. It provides an excellent introduction to handling file uploads, processing images, and organizing metadata effectively.

### Prerequisites

Before getting started with the image processing application, ensure that the following software is installed on your system:

1. **Git:** Required to clone the repository via command line from GitHub (optional if downloading the repository from the GitHub web interface as a ZIP file). [Download Git](https://git-scm.com/downloads)
2. **Python:** The application is built and tested with Python 3.11; therefore, it is recommended to use this version. However, it should also be compatible with the latest versions of Python. [Download Python](https://www.python.org/downloads/)
3. **Pip:** Required to install the Python dependencies. Pip typically comes bundled with Python, but make sure it's installed and up to date (`pip --version`). [Pip Installation Guide](https://pip.pypa.io/en/stable/installation/)
4. **Docker:** If you prefer to run the application inside a Docker container. [Download Docker](https://www.docker.com/products/docker-desktop/)
5. **IDE:** You can use any IDE to edit and navigate the code, but **Visual Studio Code** is recommended for its flexibility and extensive range of helpful extensions (e.g., Docker) that enhance development and troubleshooting. [Download Visual Studio Code](https://code.visualstudio.com/)

### Cloning the Application from GitHub

To get started with the image processing application, follow these steps to clone the repository and set it up locally:

1. Clone the repository from GitHub:
```bash
git clone https://github.com/Oxford-Research-Cloud-Competency-Centre/image-processing-app.git
```

2. Navigate to the project directory:
```bash
cd image-processing-app
```

After cloning the repository, open the project in your preferred IDE. Take some time to explore the project structure and get familiar with each component. The next section will provide an overview of the application, and afterward, we'll dive into the details of running it, covering each part of the setup and execution process.

### Application Overview

The application consists of three main components:

1. **`app.py`** - Manages the Flask web interface, providing endpoints for file uploads and image downloads, while handling image processing and metadata storage.
2. **`image_processor.py`** - Processes images by converting them to grayscale and resizing them.
3. **`database.py`** - Manages the SQLite database, storing metadata for original and processed images.

These components work together to create a cohesive image processing system.

### Core Components of the Image Processing Application

We will now discuss these components in detail, outlining their roles, and how they work together to form the complete image processing application.

#### 1. **`app.py`: Flask Application**

The central component, `app.py`, handles the core functionality of the web interface, routing, and interactions with both the image processing module and the database. It manages the following:

- **File Uploads**: Users upload images through the web interface, and these images are saved in the `images/uploads` directory. The images are stored directly on the system running the application, ensuring that the raw files are accessible for processing.
- **Image Processing**: The component calls the `process_image()` function from `image_processor.py` to process images.
- **Directory Management**: The component ensures the `images/uploads` and `images/processed` directories exist, creating them if necessary.
- **Routing**: Manages routes for displaying metadata, handling uploads, and downloading processed images:
  - `/`: Displays a dashboard with image metadata.
  - `/upload`: Handles the image upload and processing.
  - `/processed/<filename>`: Enables downloading of processed images.

#### 2. **`image_processor.py`: Image Processing**

This module handles the image transformation, performing two operations:

- **Grayscale Conversion**: Converts the uploaded image to grayscale using [OpenCV](https://opencv.org/).
- **Resizing**: Resizes the grayscale image to 300x300 pixels.

The processed images are stored in the `processed` directory.

#### 3. `database.py`: SQLite Database Management

The `database.py` module handles interactions with the SQLite database, which stores essential metadata for each uploaded and processed image. Key metadata fields include original and processed filenames, image sizes, upload dates, and optional descriptions, providing a comprehensive record of each image.

**Main functions in `database.py`:**

- **`init_db()`**: Initializes the database by creating the `ImageMetadata` table if it doesn't already exist. The `metadata.db` file is automatically generated in the project directory upon initialization.
- **`insert_metadata()`**: Adds metadata for both original and processed images, supporting detailed tracking.
- **`get_metadata()`**: Retrieves stored metadata for display on the application's dashboard.

Once an image is processed, `app.py` saves it in the `images/processed` directory on the system. Both `uploads` and `processed` directories are automatically created when the application runs, if they don't already exist.

By managing and retrieving image metadata efficiently, `database.py` enables the application to track details like:

- Filenames for original and processed images.
- Image sizes.
- Upload date and time.
- Optional descriptions.

This setup ensures efficient metadata storage and retrieval, contributing to the application's smooth functionality and user experience.

### Running the Application

The application can be run in two different ways, depending on your setup:

#### 1. **Running Locally with Python:**

To run the application locally on your system:

1. Make sure the repository is cloned and you are inside the home directory of the application.
2. Install the necessary Python dependencies:
   ```bash
   pip install -r requirements.txt
   ```
3. Run the Flask application:
   ```bash
   python app.py
   ```

The application will be accessible at http://localhost:5000

#### 2. **Running with Docker:**

Alternatively, you can run the application inside a Docker container:

1. Build the Docker image:
   ```bash
   docker build -t image-processing-app .
   ```
2. Run the Docker container:
   ```bash
   docker run -p 5000:5000 image-processing-app
   ```
This will make the application available at http://localhost:5000, without the need to install Python or any other dependencies directly on your machine.

### Application Workflow

1. **Image Upload:** Users upload images via a form on the main page. These images are saved in the `uploads` directory.
2. **Image Processing:** After an image is uploaded, the `image_processor.py` module converts it to grayscale and resizes it. The processed image is stored in the `processed` directory.
3. **Metadata Management:** Metadata for both the original and processed images—such as filename, size, and upload date—is stored in the SQLite database. This information is viewable on the application's dashboard.
4. **Download Processed Images:** Users can download the processed images directly from the dashboard.

This locally hosted image processing application provides an introduction to file uploads, image processing, and database management. It integrates **Flask** for web routing and user interaction, **OpenCV** for image processing, and **SQLite** for managing image metadata. In later sections of this course, we will expand the application by transitioning from local storage and databases to AWS services such as **S3** and **DynamoDB**, creating a scalable, cloud-native solution.
