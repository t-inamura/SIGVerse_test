#include <Ogre.h>

#define SHARED_NAME "SIGViewer-FileMapping_SceneManager"

#define SCENE_MANAGER_SIZE sizeof(Ogre::SceneManager*)

static HANDLE filemap = NULL;

void setSharedSceneManager(Ogre::SceneManager *manager);
void getSharedSceneManager(Ogre::SceneManager *manager);

void freeSharedSceneManager(Ogre::SceneManager *manager);


