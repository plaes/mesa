/*
 * Copyright (c) 2012-2015 Etnaviv Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Wladimir J. van der Laan <laanwj@gmail.com>
 */

#include "etnaviv_compiler.h"
#include "etnaviv_debug.h"
#include "etnaviv_disasm.h"
#include "tgsi/tgsi_strings.h"

extern const char *tgsi_swizzle_names[];
void
etna_dump_shader(const struct etna_shader_variant *shader)
{
   if (shader->stage == MESA_SHADER_VERTEX)
      printf("VERT\n");
   else
      printf("FRAG\n");


   etna_disasm(shader->code, shader->code_size, PRINT_RAW);

   printf("num loops: %i\n", shader->num_loops);
   printf("num temps: %i\n", shader->num_temps);
   printf("immediates:\n");
   for (int idx = 0; idx < shader->uniforms.imm_count; ++idx) {
      printf(" [%i].%s = %f (0x%08x) (%d)\n",
             idx / 4,
             tgsi_swizzle_names[idx % 4],
             *((float *)&shader->uniforms.imm_data[idx]),
             shader->uniforms.imm_data[idx],
             shader->uniforms.imm_contents[idx]);
   }

   if (DBG_ENABLED(ETNA_DBG_NIR)) {
      printf("inputs:\n");
      for (int idx = 0; idx < shader->infile.num_reg; ++idx) {
         printf(" [%i] name=%s comps=%i\n", shader->infile.reg[idx].reg,
                (shader->stage == MESA_SHADER_VERTEX) ?
                  gl_vert_attrib_name(shader->infile.reg[idx].slot) :
                  gl_varying_slot_name(shader->infile.reg[idx].slot),
                shader->infile.reg[idx].num_components);
      }
      printf("outputs:\n");
      for (int idx = 0; idx < shader->outfile.num_reg; ++idx) {
         printf(" [%i] name=%s comps=%i\n", shader->outfile.reg[idx].reg,
                (shader->stage == MESA_SHADER_VERTEX) ?
                  gl_varying_slot_name(shader->outfile.reg[idx].slot) :
                  gl_frag_result_name(shader->outfile.reg[idx].slot),
                shader->outfile.reg[idx].num_components);
      }
   } else {
      printf("inputs:\n");
      for (int idx = 0; idx < shader->infile.num_reg; ++idx) {
         printf(" [%i] name=%s index=%i comps=%i\n", shader->infile.reg[idx].reg,
                tgsi_semantic_names[shader->infile.reg[idx].semantic.Name],
                shader->infile.reg[idx].semantic.Index,
                shader->infile.reg[idx].num_components);
      }
      printf("outputs:\n");
      for (int idx = 0; idx < shader->outfile.num_reg; ++idx) {
         printf(" [%i] name=%s index=%i comps=%i\n", shader->outfile.reg[idx].reg,
                tgsi_semantic_names[shader->outfile.reg[idx].semantic.Name],
                shader->outfile.reg[idx].semantic.Index,
                shader->outfile.reg[idx].num_components);
      }
   }
   printf("special:\n");
   if (shader->stage == MESA_SHADER_VERTEX) {
      printf("  vs_pos_out_reg=%i\n", shader->vs_pos_out_reg);
      printf("  vs_pointsize_out_reg=%i\n", shader->vs_pointsize_out_reg);
      printf("  vs_load_balancing=0x%08x\n", shader->vs_load_balancing);
   } else {
      printf("  ps_color_out_reg=%i\n", shader->ps_color_out_reg);
      printf("  ps_depth_out_reg=%i\n", shader->ps_depth_out_reg);
   }
   printf("  input_count_unk8=0x%08x\n", shader->input_count_unk8);
}
