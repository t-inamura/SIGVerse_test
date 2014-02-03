#include "RenderService.h"

void setSharedSceneManager(Ogre::SceneManager *manager) {

    filemap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 
                                SCENE_MANAGER_SIZE, SHARED_NAME);
    if (filemap != NULL){
		Ogre::SceneManager *shared = (Ogre::SceneManager*)MapViewOfFile(filemap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (shared != NULL) {
			memcpy(shared, manager, sizeof(Ogre::SceneManager*));
		} else {
            MessageBox(NULL, ("Createring MapView failure"), ("Attention"), MB_OK);
		}
	} else {
        MessageBox(NULL, ("Getting FileMapping failure"), ("Attention"), MB_OK);
	}

}

void getSharedSceneManager(Ogre::SceneManager *manager) {

    filemap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 
                                SCENE_MANAGER_SIZE, SHARED_NAME);
    if (filemap != NULL){
		Ogre::SceneManager *shared = (Ogre::SceneManager*)MapViewOfFile(filemap, FILE_MAP_WRITE, 0, 0, 0);
        if (shared != NULL) {
            manager = shared;
		} else {
            MessageBox(NULL, ("Getting MapView failure"), ("Attention"), MB_OK);
		}
	} else {
        MessageBox(NULL, ("Getting FileMapping failure"), ("Attention"), MB_OK);
	}
    manager = NULL;
}

void freeSharedSceneManager(Ogre::SceneManager *manager) {
    if (filemap != NULL) {
		if (manager != NULL) {
            UnmapViewOfFile(manager);
		}
        CloseHandle(filemap);
        filemap = NULL;
    }
}

