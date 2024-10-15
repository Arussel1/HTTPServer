# Simple C++ Web Server

A lightweight web server built in C++ that supports basic HTTP functionalities such as serving static files, handling authentication, and allowing file uploads. This project is a part of my journey to learn web development and enhance my understanding of networking concepts.

## Features

- **Authentication**: Users can log in with a username and password.
- **File Upload**: Users can upload files to the server.
- **Rate Limiting**: Limits the number of requests from clients to prevent abuse.

## Getting Started

### Prerequisites

- C++ Compiler (e.g., g++)
- CMake (optional, for easier builds)

### Installation

1. Clone the repository:
   ```bash
   git clone git@github.com:Arussel1/HTTPServer.git
   cd HTTPServer
2. Compile the server:
    ```bash
    g++ main.cpp -o webserver -I./include
    ```
3. Run the server:
    ```bash
    ./webserver
    ```
4. Access the server in your web browser or use cURL or use Postman:
    * Navigate to http://localhost:8080 to access the server.

## TODO List

### File Management
- [ ] Implement file listing functionality to show uploaded files.
- [ ] Allow users to delete or download files.

### User Sessions
- [ ] Improve session management to keep users logged in and manage their sessions securely.
- [ ] Store session data in memory or in a database.

### Improved Security
- [ ] Hash passwords before storing or processing them.
- [ ] Implement HTTPS for secure communication.

### Error Handling
- [ ] Enhance error handling for various scenarios (e.g., file type checks, file size limits, etc.).
- [ ] Provide user-friendly error messages.

### Frontend Improvements
- [ ] Create a simple frontend interface using HTML/CSS/JavaScript to make interactions easier (e.g., login forms, file upload forms).

### Database Integration
- [ ] Consider integrating a database to store user data and uploaded file metadata.

### Rate Limiting
- [ ] Refine the rate-limiting implementation to prevent abuse, particularly for the login and file upload routes.

### Testing
- [ ] Write automated tests for server functionality.

## License

This project is licensed under the MIT License - see the LICENSE file for details.