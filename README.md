# Gyaan Srot

## Overview

**Gyaan Srot** is a cross-platform application that allows users to parse, manage, and view articles from various RSS feeds. It offers a user-friendly GUI for browsing through categories and sources, as well as the ability to install new sources and fetch the latest articles.


## Features

- **Category & Source Management**: Install, uninstall, and manage sources categorized by topics.
- **Article Viewing**: Browse and read articles in a sleek, integrated web view.
- **Filtering and Sorting**: Sort articles by date, title, and filter by time (today, yesterday, etc.).
- **Search Functionality**: Search through articles based on keywords.

## Screenshots

| Startup Screen | Install New Sources |
|--------------|--------------------|
|![startupscreen](https://github.com/user-attachments/assets/0cdd4fce-c401-4136-92b7-b594b4e5d7a4)|![install sources](https://github.com/user-attachments/assets/deb89e4e-2bc7-4c41-9fe2-16008dc3c4d9)|


| Read Articles | Filter Articles | 
|---------------|-----------------|
|![read article](https://github.com/user-attachments/assets/40e0725d-0d1b-49f9-9656-a0d99cfa2c8a)|![filter](https://github.com/user-attachments/assets/9442112b-e665-41db-9694-c850226d96e5)|

| Sort Articles | Search Articles |
|---------------|-----------------|
|![sort](https://github.com/user-attachments/assets/8ea436c9-791f-473d-98a4-c2fbc799c901)|![search](https://github.com/user-attachments/assets/ef11d67c-6698-429e-921e-b24469711412)|


## Requirements

- **Qt 6.x or higher**
- **CMake 3.10 or higher**
- **GCC/Clang**

## Installation

1. **Clone the Repository:**
    ```bash
    git clone https://github.com/nivassai03/GyaanSrot.git
    cd GyaanSrot
    ```

2. **Build the Project:**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

3. **Run the Application:**
    ```bash
    ./gyaansrot
    ```
    
## Usage

### Adding and Removing Sources

- **Install Sources:** Use the "Install Sources" option from the toolbar to install new RSS sources.
- **Uninstall Sources:** Uninstall existing sources from the "Installed Sources" tab.

### Browsing Articles

- **View Articles:** Select a source to view its articles. Use the filter and sort options to refine the list.
- **Search Articles:** Use the search bar to find articles containing specific keywords.

### Fetching Articles

- The application automatically fetches new articles when a source is installed. You can also manually refresh the sources to fetch new articles.

## Credits
GyaanSrot is written in c++17 and uses the following software.
- [Qt](https://www.qt.io/)
- [Cmake](https://cmake.org/)


