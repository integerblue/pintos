#include <hash.h>
#include "vm/page.h"
#include "threads/thread.h"

void spt_init (struct hash *spt) {
  hash_init (spt, spt_hash_func, spt_less_func, NULL);
}

unsigned spt_hash_func (const struct hash_elem *e, void *aux UNUSED) {
  struct spte *spte = hash_entry (e, struct spte, hash_elem);
  return hash_int((int)(spte->va));
}

bool spt_less_func (const struct hash_elem *a, const struct hash_elem *b, void *aux UNUSED) {
  struct spte *spte_a = hash_entry (a, struct spte, hash_elem);
  struct spte *spte_b = hash_entry (b, struct spte, hash_elem);
  return spte_a->va < spte_b->va;
}

void spt_insert (void *vaddr) {
  struct thread *cur = thread_current ();
  struct spte spte;
  spte.va = vaddr;
  hash_insert (&cur->spt, &spte.hash_elem);
}

void spt_delete (void *vaddr) {
  struct thread *cur = thread_current ();
  struct spte spte;
  spte.va = vaddr;
  hash_delete (&cur->spt, &spte.hash_elem);
}

void spt_destroy (void) {
  struct thread *cur = thread_current ();
  if (&cur->spt != NULL) {
    hash_destroy (&cur->spt, NULL);
  }
}
