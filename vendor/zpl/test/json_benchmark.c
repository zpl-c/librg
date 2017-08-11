//
// Requires presence of jeopardy.json file, which can be received at: https://www.reddit.com/r/datasets/comments/1uyd0t/200000_jeopardy_questions_in_a_json_file/
//
// It took 162 milliseconds to process this JSON file on my computer.
// UPDATE: Parsing takes around 86 milliseconds on i7-4790k.
//
#define ZPL_IMPLEMENTATION
#define ZPLJ_IMPLEMENTATION
#include <zpl.h>
#include <zpl_json.h>

int main(void) {

    zpl_file_t file;
    zpl_file_open(&file, "jeopardy.json");
    isize file_size = zpl_file_size(&file);
    char *content = zpl_malloc(file_size+1);
    zpl_file_read(&file, content, file_size);
    content[file_size] = 0;
    zpl_file_close(&file);


    zplj_object_t root = {0};

    f64 time = zpl_time_now();
    zplj_parse(&root, file_size, content, zpl_heap_allocator(), false);
    f64 delta = zpl_time_now() - time;

    printf("Delta: %fms\n", delta*1000);

    printf("No. of nodes: %td\n", zpl_array_count(root.nodes[0].elements));

    printf("Category: %s, air date: %s\nQuestion: %s\n", root.nodes[0].elements[29].nodes[0].string,
           root.nodes[0].elements[29].nodes[1].string,
           root.nodes[0].elements[29].nodes[2].string);

    zplj_free(&root);

    zpl_mfree(content);
    return 0;
}
