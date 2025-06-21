import os
from huggingface_hub import InferenceClient
from transformers import pipeline
from openai import OpenAI

# 1) Create the QA pipeline (this will automatically use the HF Inference API if no model is cached)


# 2) Your context paragraphs and question
context = (
    open("paragraph_0.txt").read() + "\n\n" +
    open("paragraph_1.txt").read() +"\n\n" +
    open("paragraph_2.txt").read()
)
question = open("query.txt").read().strip()

# question += "answer: Gandhi's views on the manner of how one should eat were..."

# 1. Set your token in the current process’s env
context +="\n\nQuestion: "
context += question


client = OpenAI(
  api_key="your_api_key"
)

completion = client.chat.completions.create(
  model="gpt-4o-mini",
  store=True,
  messages=[
    {"role": "user", "content": context}
  ]
)

print(completion.choices[0].message);

print("Answer:", completion)
print("Score:", completion)

# OTHERWAY 

# from transformers import AutoModelForCausalLM, AutoTokenizer, pipeline
# import torch
# import sys

# if __name__ == '__main__':
#     if len(sys.argv) < 3:
#         print("Usage: python api_call.py num_paragraphs query.txt")
#         sys.exit(1)

#     num_paragraphs = int(sys.argv[1])
#     query_file      = sys.argv[2]

#     # Load paragraphs
#     paragraphs = ["Context:\n"]
#     for i in range(num_paragraphs):
#         with open(f"paragraph_{i}.txt") as f:
#             paragraphs.append(f.read())
#             paragraphs.append("\n")

#     question = open(query_file).read().strip()

#     # Reformulate using T5 model (uses GPU if available)
#     reformulator = pipeline("text2text-generation",
#                             model="domenicrosati/QA2D-t5-base",
#                             device=0 if torch.cuda.is_available() else -1)
#     stub = reformulator(question, max_length=50)[0]["generated_text"]

#     # Build prompt
#     paragraphs.append("\nQuestion: " + question)
#     paragraphs.append("Answer: "    + stub + " ...")
#     prompt = "\n".join(paragraphs)
#     print("=== Prompt ===\n", prompt)

#     # Load GPT-Neo and move to GPU
#     model_name = "EleutherAI/gpt-neo-1.3B"
#     tokenizer  = AutoTokenizer.from_pretrained(model_name)
#     model      = AutoModelForCausalLM.from_pretrained(model_name)

#     device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
#     model = model.to(device)

#     # Tokenize and send to GPU
#     inputs = tokenizer(prompt, return_tensors="pt",
#                        truncation=True, max_length=2048)
#     inputs = {k: v.to(device) for k, v in inputs.items()}

#     # Generate output
#     outputs = model.generate(**inputs, max_new_tokens=200,
#                              pad_token_id=tokenizer.eos_token_id)

#     reply = tokenizer.decode(outputs[0], skip_special_tokens=True)
#     print("=== Model Reply ===\n", reply)

#     with open("answer.txt", "w") as fout:
#         fout.write(reply)
