# Destructors:

  Each class can only have one destructor

  Example:
```
    class SomeClass {
      public:
      ~SomeClass()
        {
        // your destructor
        // code goes here
        }
      private: …
    };
```
  Destructors must not return a value.

  C++ will define an implicit destructor if one is not declared.

  Why are destructors needed?

  Example:
```
    class MP3Player
    {
    public:
      MP3Player()
        {
          reserveSpaceOnDisk(100000000); // 100MB
        }

        void getSong(const std::string &songURL)
        {
          downloadToReservedSpace(songURL);
        }

        void playSong()
        {
          playMP3InReservedSpace();
        }

        void doneWithMusic()
        {
          freeReservedSpaceOnDisk();
        }
      };

      int main()
      {
        string url = “www.music.com/song.mp3”;
        // obsessively play song 100 times!
        for (int i=0;i<100;i++)
        {
          MP3Player p;
          p.getSong(url);
          p.playSong();
        }
      }
```

  Problem - disk space is not freed when p goes out of scope. This can be
  solved with a user declared destructor that frees the disk space:

```
      ~MP3Player()
      {
        freeReservedSpaceOnDisk();
      }
```
  Any time a class allocates a system resource, the destructor must deallocate
  the resource.

  Destructors are called when objects go out of scope or when delete is called
  on a dynamically allocated object
