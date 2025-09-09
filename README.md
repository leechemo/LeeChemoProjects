Recipes Project- Client-Server Side Programming:
This project is a full-stack web application for managing recipes. 
It includes user authentication, personal and family recipe creation, favorites, and meal planning features. 
The backend uses Node.js, Express, and MySQL, while the frontend is built with Vue.js and Bootstrap-Vue for responsive design. 
It also integrates with an external recipes API (Spoonacular) to fetch and display additional recipe data.
The system tracks user activity, such as viewed recipes and preparation progress. 
*** Recording is attached.

Super-Li Project:
The project is a Java-based delivery and workforce management system for a chain of stores. 
It includes modules for managing deliveries, drivers, HR managers, and company employees, with features such as creating and updating deliveries, 
generating reports, managing employee shifts, and simulating daily operations. 
The system uses SQLite for data storage and provides command-line menus for different user roles.

Parallel text search engine in modern C++:
This project enumerates files (optionally recursively), pushes them into a thread-safe task queue, and uses a worker thread pool to scan files in parallel for a given pattern (plain text or regex), with optional case-insensitive matching, binary-file skipping, extension filters, and a stop-on-first shortcut. Each worker collects matches locally and a central stage merges results efficiently, supporting clean cancellation via an atomic flag. Results can be exported in multiple formats—human-readable table, CSV, or JSON. The code is modular: FileEnumerator (collect files with std::filesystem), FileScanner (line-by-line search), ThreadSafeQueue (mutex + condition_variable), WorkerPool (threads + merging), OutputWriter (formatting), and SearchConfig (CLI options).

Song Popularity Prediction – Machine Learning Project:
Developed a machine learning pipeline to predict song popularity using audio features, metadata, and BERT-based lyric embeddings. 
Applied advanced preprocessing, feature engineering, and model optimization (Random Forest, XGBoost), achieving 88% accuracy with Random Forest.
