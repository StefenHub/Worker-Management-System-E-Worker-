# Worker Management System
A C++ console application for managing worker records, designed to handle employee data with features like adding, editing, deleting, searching, and displaying workers. The system includes a user-friendly interface with a logo display, screen clearing, and interactive prompts, tailored for Windows environments.

## Features
- **Manage Workers**:
  - Store ID ("wXXXXX" format), name, gender (M/F), position, wage, and hours.
  - Calculate Gross Salary, Tax (25% female, 30% male if gross ≥ $1000), and Net Salary.
- **Operations**:
  - **Add**: Add workers with auto-generated IDs and "y/n" prompt to continue.
  - **Edit**: Update by ID or name, skip fields with Enter, case-insensitive name search.
  - **Delete**: Remove by ID or name, select ID for multiple name matches.
  - **Search**: Find by ID or name, retry until found or exit with 'e'.
  - **Display**: Show workers sorted by ID or Net Salary.
