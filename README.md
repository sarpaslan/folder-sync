# folder-sync
Syncs two folders between them.


Usage:
sync <source_folder> <target_folder>

If there is no target folder, it will be created.

Copies files from source to target if they do not exist in the target folder.

If any file in the target folder is newer than its counterpart in the source folder, it will be copied from the target folder to the source folder too. So its 2 way sync. 

This is not platform-independent and likely contains several bugs and its not even a good solution.
