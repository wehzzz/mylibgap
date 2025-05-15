FROM python:3.11-slim

RUN apt-get update && \
    apt-get install -y doxygen make

RUN pip install --no-cache-dir sphinx breathe sphinx-rtd-theme

WORKDIR /docs

COPY script.sh /docs/script.sh
RUN chmod +x /docs/script.sh

CMD ["bash", "/docs/developer-guide/script.sh"]
# CMD ["bash"]

