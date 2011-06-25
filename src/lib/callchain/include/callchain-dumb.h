struct fcall_chain_t
{
  struct fcall_chain_t* next;
  void (*func)(void);
};