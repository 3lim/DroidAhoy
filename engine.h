class Engine{
  private:

    Engine();
    ~Engine();
    operator=Engine();
    
  public:
  
    static int init();
    static int render();
    static int update();

  protected:  
};
